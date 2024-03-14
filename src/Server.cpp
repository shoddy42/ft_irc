/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2024/03/06 04:04:21 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Server::Server(void)
{
	time_t now = time(0);
	_creation_time = ctime(&now);
	if (!_creation_time.empty() && _creation_time[_creation_time.length() - 1] == '\n')
		_creation_time.pop_back();
	// _unfinished_packets.resize(MAX_CLIENTS + 1);
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
 * 		  Also creates null channel and user to guarantee that the lists arent empty.
 * @param port Port to bind socket to
 * @return int (the socket FD)
 */
void	Server::start(int port, std::string password)
{
	_listen_socket = guard(socket(AF_INET, SOCK_STREAM, 0), "Failed to create socket. errno:");
	// guard(fcntl(_listen_socket, F_SETFL, O_NONBLOCK), "Failed to set socket to non-blocking. errno: ");
	
	sockaddr_in sock_address;
	sock_address.sin_port = htons(port);
	sock_address.sin_family = AF_INET;
	sock_address.sin_addr.s_addr = htonl(INADDR_ANY);

	int opt = 1;
	guard(setsockopt(_listen_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)), "Failed to set socket to reusable. errno: ");
	guard(bind(_listen_socket, (struct sockaddr*)&sock_address, sizeof(sockaddr_in)), "Failed to bind to port. errno: ");
	guard(listen(_listen_socket, MAX_CLIENTS + 1), "Failed to listen on socket. errno: ");
   
	pollfd listen_socket;
	listen_socket.fd = _listen_socket;
	listen_socket.events = POLLIN;
	pollfds.push_back(listen_socket);

	Channel null_channel(NULL_CHANNEL_NAME, *this);
	channels.push_back(null_channel);

	User null_user(-42);
	users.push_back(null_user);

	_password = password;
}

/**
 * @brief The main loop. Polls all sockets. Then receives and responds when they are ready.
 */
void	Server::serve(void)
{
	//Poll every socket we have.
	poll(pollfds.data(), pollfds.size(), POLL_TIMEOUT);
	accept_new_connection();
	for (size_t i = 1; i < pollfds.size(); i++)
	{
		if (pollfds[i].revents == 0) //client hasnt done anything
			continue;
		if (pollfds[i].revents & POLLHUP) //client disconnected
		{
			remove_user(get_user(pollfds[i].fd));
			_unfinished_packets[i].clear();
		}
		else if (pollfds[i].revents & POLLIN) //client sent server a message
		{
			_unfinished_packets[i] += receive(i);
			if (_unfinished_packets[i].find(std::string("\n")) == std::string::npos)
				std::cout << RED << "Received Unfinished packet [" << _unfinished_packets[i] << "]" << std::endl << RESET;
			else
			{
				do_command(_unfinished_packets[i], get_user(pollfds[i].fd));
				_unfinished_packets[i].clear();
			}
			pollfds[i].revents = 0;
		}
		else if (pollfds[i].revents & POLLOUT) //client is ready for a response
			respond(get_user(pollfds[i].fd));
	}
}

std::string	Server::receive(int sock)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read;

	bzero(buffer, BUFFER_SIZE);
	bytes_read = recv(pollfds[sock].fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read == -1)	  // Recv failed
		return ("");
	else if (bytes_read == 0) // Connection closed by the client
		remove_user(get_user(pollfds[sock].fd));
	else  				      // Actually received a message
		return (std::string(buffer));
	return ("");
}

static bool border_patrol(std::string response, User &user, Server &server)
{
	std::string disconnect_msg;
	if (response == "462 :Unauthorized command (already registered)\r\n")
	{
		std::cout << "Registered user getting booted!\n";
		std::string disconnect_msg = "ERROR :You have been kicked from the server (Reason: Account Already Registered)." ;
	}
	else if (response == "464 * :Password incorrect!\r\n")
	{
		std::cout << "Unregistered user getting booted!\n";
		std::string disconnect_msg = "ERROR :You have been kicked from the server (Reason: Invalid password)." ;
	}
	else if (response == "464 * :Please provide a password.\r\n")
	{
		std::cout << "Unregistered user getting booted!\n";
		std::string disconnect_msg = "ERROR :You have been kicked from the server (Reason: Please send PASS first)." ;
	}
	send(user.get_socket(), disconnect_msg.c_str(), disconnect_msg.length(), 0);
	server.remove_user(user);
	return (true);
}

void	Server::respond(User &user)
{
	while (user.check_response() > 0)
	{
		std::string response = user.give_response();
		if (response.empty())
			continue;
		std::cout << user.get_nickname() << " (" << user.get_username() << ")" " >> " << YELLOW << response << RESET << std::endl;
		response += "\r\n";
		if (border_patrol(response, user, *this) == false)
			break;
		send(user.get_socket(), response.c_str(), response.length(), 0);
	}
}

/**
 * @brief Creates and executes all commands within the buffer.
 * 
 * @param buffer The received data from server.receive()
 */
void	Server::do_command(std::string buffer, User &caller)
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
	for (size_t i = 0; i < pollfds.size() && i < MAX_CLIENTS; i++)
		if (pollfds[i].fd > 0)
			socket_cleanup(pollfds[i].fd);
}

void	Server::accept_new_connection(void)
{
	if (pollfds[0].revents & POLLIN)
	{
		std::cout << "New connection found." << std::endl;
		int client_socket = guard(accept(pollfds[0].fd, nullptr, nullptr), "Failed to accept socket. errno: ");
		// guard(fcntl(client_socket, F_SETFL, O_NONBLOCK), "Failed to set socket to non-blocking. errno: ");
		add_user(client_socket);
	}
}

/**
 * @brief Closes socket, and updates pollfds and the users socket to -1 (INVALID_FD) if needed
 */
void	Server::socket_cleanup(int sock)
{
	if (sock == INVALID_FD || sock == 0)
		return;
	std::cout << PURPLE << "Socket cleanup on aisle " << sock << RESET << std::endl;
	close(sock);
	for (std::list<User>::iterator it = users.begin(); it != users.end(); it++)
	{
		if (it->get_socket() == sock)
		{
			it->set_socket(INVALID_FD);
			break;
		}
	}

	for (size_t i = 0; i < pollfds.size(); i++)
	{
		if (pollfds[i].fd == sock)
		{
			pollfds[i].fd = INVALID_FD;
			pollfds[i].revents = 0;
			if (_unfinished_packets.size() < i)
				_unfinished_packets[i].clear();
		}
	}
}

void	Server::remove_user(User &user_to_delete)
{
	if (user_to_delete.get_socket() == -42)
		return;
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

void Server::add_channel(std::string name, User &creator)
{
	if (name[0] != '#' && name[0] != '&' && name[0] != '!' && name[0] != '+')
	{
		std::cout << RED <<  "Invalid channel prefix!" << RESET << std::endl;
		return;
	}
	std::cout << PURPLE <<  "Creating channel " << name << RESET << std::endl;
	Channel new_channel(name, *this);
	new_channel.add_operator(creator);
	new_channel.add_invited(creator);
	new_channel.add_user(creator);
	std::string reply = usermask(creator) + " JOIN :" + new_channel.get_name();
	creator.add_response(reply);
	channels.push_back(new_channel);
}

void Server::add_user(int sock)
{
	pollfd new_fd;
	new_fd.fd = sock;
	new_fd.events = POLLIN | POLLOUT;
	new_fd.revents = 0;

	bool added_user = false;

	for (size_t i = 1; i < pollfds.size(); i++) //check if any of the old pollfds have since left.
	{
		if (pollfds[i].fd == INVALID_FD)
		{
			pollfds[i] = new_fd;
			added_user = true;
			break;
		}
	}
	if (added_user == false) //if there is no reusable space, push more space instead.
	{
		if (pollfds.size() >= MAX_CLIENTS + 1)
		{
			std::cout << RED << "Server is full." << END_LINE;
			socket_cleanup(sock);
			return;
		}
		pollfds.push_back(new_fd);
	}
	if (pollfds.size() > _unfinished_packets.size())
		_unfinished_packets.resize(pollfds.size());
	User new_user(sock);
	users.push_back(new_user);
}

void Server::remove_channel(Channel &channel)
{
	if (channel.get_name() == NULL_CHANNEL_NAME)
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

std::string	&Server::get_creation_time(void)
{
	return (_creation_time);
}

/* ************************************************************************** */