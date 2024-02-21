/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2024/02/21 16:11:28 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(void)
{
}

Server::Server(const Server &src)
{
	if (this != &src)
		*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Server::~Server(void)
{
	// this->shutdown();
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &Server::operator=(Server const &src)
{
	if (this == &src)
		return (*this);
	pollfds = src.pollfds;
	users = src.users;
	channels = src.channels;
	_listen_socket = src._listen_socket;
	_password = src._password;
	return (*this);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/**
 * @brief Creates a new socket to listen on port. Configures server.poll[0] to be this new socket.
 * 		  Also creates dummy channels and general to guarantee that the lists arent empty.
 * @param port Port to bind socket to
 * @return int (the socket FD)
 */
void	Server::start(int port, std::string password)
{
	_listen_socket = guard(socket(AF_INET, SOCK_STREAM, 0), "Failed to create socket. errno:");
	guard(fcntl(_listen_socket, F_SETFL, O_NONBLOCK), "Failed to set socket to non-blocking. errno: ");
	
    sockaddr_in sock_address;
	sock_address.sin_port = htons(port);
    sock_address.sin_family = AF_INET;
    sock_address.sin_addr.s_addr = INADDR_ANY;

	int opt = 1;
    guard(bind(_listen_socket, (struct sockaddr*)&sock_address, sizeof(sockaddr_in)), "Failed to bind to port. errno: ");
	guard(setsockopt(_listen_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)), "Failed to set socket to reusable");
	guard(listen(_listen_socket, MAX_CLIENTS), "Failed to listen on socket. errno: ");
   
	pollfd listen_socket;
	listen_socket.fd = _listen_socket;
	listen_socket.events = POLLIN;
	pollfds.push_back(listen_socket);

	Channel dummy_channel("dummy channel", *this);
	channels.push_back(dummy_channel);

	User dummy_user(-42);
	users.push_back(dummy_user);

	_password = password;
}

/**
 * @brief The main loop. Polls all sockets. Then receives and responds when they are ready.
 */
void	Server::serve(void)
{
	//Poll every socket we have.
	//Check all open sockets, to see if any have written to us.
	poll(pollfds.data(), pollfds.size(), POLL_TIMEOUT);
	accept_new_connection();
	for (size_t i = 1; i < pollfds.size(); i++)
	{
		if (pollfds[i].revents == 0) //client hasnt done anything
			continue;
		if (pollfds[i].revents & POLLHUP) //client disconnected
			delete_user(get_user(pollfds[i].fd));
		else if (pollfds[i].revents & POLLIN) //client sent server a message
			create_command(receive(i), get_user(pollfds[i].fd));
		else if (pollfds[i].revents & POLLOUT && i > 0) //client is ready for a response
			respond(get_user(pollfds[i].fd));
	}
}

std::string	Server::receive(int sock)
{
	// std::cout << "receiving from socket [" << pollfds[sock].fd << "], at position (" << sock << ") AKA " << get_user(pollfds[sock].fd).get_username() << std::endl;
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read;

	bzero(buffer, BUFFER_SIZE);
	bytes_read = recv(pollfds[sock].fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read == -1)	  // Recv failed
		return ("");
	else if (bytes_read == 0) // Connection closed by the client //might not be needed, POLLHUP should already catch
		delete_user(get_user(pollfds[sock].fd));
	else  				      // Actually received a message
		return (std::string(buffer));
	return ("");
}

void	Server::respond(User &user)
{
	while (user.check_response() > 0)
	{
		std::string response = user.give_response();
		if (response.empty())
			break;
		std::cout << user.get_nickname() << " (" << user.get_username() << ")" " >> " << YELLOW << response << RESET << std::endl;
		response += "\r\n";

		if (response == "462 :Unauthorized command (already registered)\r\n")
		{
			std::cout << "Registered user getting booted!\n";
			std::string disconnect_msg = "ERROR :You have been kicked from the server (Reason: Account Already Registered)." ;
			send(user.get_socket(), disconnect_msg.c_str(), disconnect_msg.length(), 0);
			delete_user(user);
			break;
		}
		else if (response == "464 * :Password incorrect!\r\n")
		{
			std::cout << "Unregistered user getting booted!\n";
			std::string disconnect_msg = "ERROR :You have been kicked from the server (Reason: Invalid password)." ;
			send(user.get_socket(), disconnect_msg.c_str(), disconnect_msg.length(), 0);
			delete_user(user);
			break;
		}
		send(user.get_socket(), response.c_str(), response.length(), 0);
	}
}

void	Server::create_command(std::string buffer, User &caller)
{
	if (buffer.empty())
		return;

	std::stringstream ss(buffer);
	std::string line;
	while (std::getline(ss, line, '\n'))
	{
		Command command(*this, caller);
		std::cout << caller.get_nickname() << " (" << caller.get_username() << ") << " << BLUE << line << RESET << std::endl;
		if (!line.empty())
		{
			std::istringstream split(line);
			std::string token;
			while (split >> token)
				command.add_argument(token);
		}
		command.execute();
	}
}

void	Server::shutdown(void)
{
	for (size_t i = 0; i < pollfds.size(); i++)
		if (pollfds[i].fd > 0)
			socket_cleanup(pollfds[i].fd);
}

/**
 * @brief Checks if our listening socket has been contacted. Then creates a user.
 * 
 */
void	Server::accept_new_connection(void)
{
	if (pollfds[0].revents & POLLIN)
	{
		std::cout << "New connection found." << std::endl;
		int client_socket = guard(accept(pollfds[0].fd, nullptr, nullptr), "Failed to accept socket. errno: ");
		guard(fcntl(client_socket, F_SETFL, O_NONBLOCK), "Failed to set socket to non-blocking. errno: ");
		add_user(client_socket);
	}
}

/**
 * @brief Closes socket, and updates pollfds and the users socket to -1 (INVALID_FD) if needed
 * 
 * @param sock The socket to cleanup.
 */
void	Server::socket_cleanup(int sock)
{

	std::cout << PURPLE << "Socket cleanup on aisle " << sock << RESET << std::endl;
	if (sock == INVALID_FD)
		return;
	close(sock);
	for (std::list<User>::iterator it = users.begin(); it != users.end(); it++)
		if (it->get_socket() == sock)
			it->set_socket(INVALID_FD);

	for (size_t i = 0; i < pollfds.size(); i++)
	{
		if (pollfds[i].fd == sock)
		{
			// std::cout << GREEN << "Socket actually cleared: " << sock << RESET << std::endl;
			pollfds[i].fd = INVALID_FD;
			pollfds[i].revents = 0;
		}
	}
}

void	Server::delete_user(User &user_to_delete)
{
	for (std::list<Channel>::iterator channel = channels.begin(); channel != channels.end(); channel++)
		if (channel->remove_user(user_to_delete, "User has left the server") == true)
			channel = channels.begin();
	socket_cleanup(user_to_delete.get_socket());
	for(std::list<User>::iterator user = users.begin(); user != users.end(); user++)
	{
		if (user->get_socket() == user_to_delete.get_socket())
		{
			users.erase(user);
			break;
		}
	}
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void Server::add_channel(std::string name)
{
	if (name[0] != '#' && name[0] != '&' && name[0] != '!' && name[0] != '+')
	{
		std::cout << "Invalid channel prefix!" << std::endl;
		return;
	}
	Channel new_channel(name, *this);
	channels.push_back(new_channel);
}

void Server::add_user(int sock)
{
	pollfd new_fd;
	new_fd.fd = sock;
	new_fd.events = POLLIN | POLLOUT;
	new_fd.revents = 0;
	pollfds.push_back(new_fd);

	User new_user(sock);
	users.push_back(new_user);
}

void Server::remove_channel(Channel &channel)
{
	if (channel.get_name() == "dummy channel")
		return;
	for(std::list<Channel>::iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (it->get_name() == channel.get_name())
		{
			std::cout << PURPLE << "Channel " << channel.get_name() << " erased!\n" << RESET;
			channels.erase(it);
			break;
		}
	}
}

Channel	&Server::get_channel(const std::string name)
{
	for (std::list<Channel>::iterator channel = channels.begin(); channel != channels.end(); channel++)
		if (channel->get_name() == name)
			return (*channel);
	std::cout << RED << "No channel " << name << " found." << std::endl << RESET;
	return (*channels.begin());
}

User &Server::get_user(std::string name)
{
	for (std::list<User>::iterator user = std::next(users.begin()); user != users.end(); user++)
		if (user->get_username() == name || user->get_nickname() == name)
			return (*user);
	std::cout << RED << "No user " << name << " found." << std::endl << RESET;
	return (*users.begin());
}

User &Server::get_user(int sock)
{
	for (std::list<User>::iterator user = std::next(users.begin()); user != users.end(); user++)
		if (user->get_socket() == sock)
			return (*user);
	// std::cout << RED << "No user is connected to sock " << sock << std::endl << RESET;
	return (*users.begin());
}

std::string	&Server::get_password(void)
{
	return (_password);
}

/* ************************************************************************** */