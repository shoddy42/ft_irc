/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2023/12/13 17:02:18 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../include/Server.hpp"
#include <iostream> //todo: delete this, its only needed for debug messages

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
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Server &Server::operator=(Server const &src)
{
	if (this == &src)
		return (*this);
	return (*this);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

std::string	&Server::get_password(void)
{
	return (_password);
}


/**
 * @brief Creates a new pollfd, configures it to check for POLLIN and POLLOUT events. Then creates a new user with this data
 * 
 * @param sock 
 */
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

User &Server::get_user(int sock)
{
	//next to skip dummy at start users
	for (std::list<User>::iterator user = std::next(users.begin()); user != users.end(); user++)
		if (user->get_socket() == sock)
			return (*user);
	return (*users.begin());
	// throw std::runtime_error("User not found");
}

User &Server::get_user(std::string name)
{
	// for (std::list<User>::iterator user = std::next(users.begin()); user != users.end(); user++)
	for (std::list<User>::iterator user = users.begin(); user != users.end(); user++)
		if (user->get_username() == name)
			return (*user);
	return (*users.begin());
	// throw std::runtime_error("User not found");
}

Channel	&Server::get_channel(const std::string name)
{
	// return (*channels.begin());
	for (std::list<Channel>::iterator channel = channels.begin(); channel != channels.end(); channel++)
		if (channel->get_name() == name)
			return (*channel);
	std::cout << RED << "No channel " << name << " found.\n" << RESET;
	return (*channels.begin());
	// throw std::runtime_error("Channel not found");
}


/**
 * @brief Creates a new socket to listen on port. Configures server.poll[0] to be this new socket.
 * 
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

    guard(bind(_listen_socket, (struct sockaddr*)&sock_address, sizeof(sock_address)), "failed to bind to port. errno: ");
	guard(listen(_listen_socket, MAX_CLIENTS), "Failed to listen on socket. errno: ");
   
	pollfd listen_socket;
	listen_socket.fd = _listen_socket;
	listen_socket.events = POLLIN;
	pollfds.push_back(listen_socket);

	Channel dummy_channel("dummy channel", *this);
	channels.push_back(dummy_channel);

	Channel general("#general", *this);
	channels.push_back(general);

	User dummy_user(-42);
	users.push_back(dummy_user);

	_password = password;
}

void	Server::shutdown(void)
{
	for (size_t i = 0; i < pollfds.size(); i++)
		if (pollfds[i].fd > 0)
			close(pollfds[i].fd);
	exit(0);
}

/**
 * @brief Checks if our listening socket has been contacted. Then creates a user.
 * 
 */
void	Server::accept_new_connection(void)
{
	if (pollfds[0].revents & POLLIN)
	{
		std::cout << "New connection found. CONNECTING" << std::endl;
		//possible here to steal client information with sockaddr instead of nullptr.
		int client_socket = guard(accept(pollfds[0].fd, nullptr, nullptr), "Failed to accept socket. errno: ");
		guard(fcntl(client_socket, F_SETFL, O_NONBLOCK), "Failed to set socket to non-blocking. errno: ");
		std::cout << "client socket created at: " << client_socket << std::endl;
		this->add_user(client_socket);
		// send(client_socket, "ERROR :Password required\r\n", 6, 0);
	}
}

/**
 * @brief Closes socket, and updates pollfds and the users socket to -1 (INVALID_FD) if needed
 * 
 * @param sock The socket to cleanup.
 */
void	Server::socket_cleanup(int sock)
{
	std::cout << GREEN << "Socket cleanup on aisle " << sock << RESET << std::endl;
	if (sock == INVALID_FD)
		return;
	close(sock);
	for (std::list<User>::iterator it = users.begin(); it != users.end(); it++)
		if (it->get_socket() == sock)
			it->set_socket(INVALID_FD);
	
	pollfds[sock].fd = INVALID_FD;
	pollfds[sock].revents = 0;
}

//todo: Make this function loop until CLRF (\r\n) is found, instead of assuming BUFFER_SIZE is enough for the whole packet
std::string	Server::receive(int sock)
{
	std::cout << "receiving from socket [" << sock << "]: "<< std::endl;
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read;

	bzero(buffer, BUFFER_SIZE);
	bytes_read = recv(pollfds[sock].fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read == -1)	  // Recv failed
		perror("recv");
	else if (bytes_read == 0) // Connection closed by the client //might not be needed, POLLHUP should already catch
	{
		std::cout << "Client closed the connection." << std::endl;
		socket_cleanup(pollfds[sock].fd);
	} 
	else  				      // Actually received a message
	{
		std::string data(buffer);
		//SCUFFED early reply to trick client into thinking its fully connected
		// if (data.find("USER") != std::string::npos && (pollfds[sock].fd & POLLOUT))
		// {
		// 	std::cout << "Responding to client" << std::endl;
		// 	std::string join_response(":localhost 001 jeff :Welcome to the IRC server, jeff!\n");
		// 	send(pollfds[sock].fd, join_response.c_str(), join_response.length(), 0);
		// }
		return (data);
	}
	return ("");
}

void	Server::respond(User &user)
{
	while (user.check_response() > 0)
	{
		std::string response = user.give_response();
		if (response.empty()) //shouldn't be needed but safety /shrug
			break;
		//todo: finish packet function that adds CRLF (\r\n) properly?
		std::cout << "Sending Packet: " << YELLOW << response << RESET << " to client " << user.get_socket() << std::endl;
		response += "\r\n";
		send(user.get_socket(), response.c_str(), response.length(), 0);
	}
}

void	Server::serve(void)
{
	//Poll every socket we have.
	//Check all open sockets, to see if any have written to us.
	guard(poll(pollfds.data(), pollfds.size(), POLL_TIMEOUT), "poll failed. errno: ");

	accept_new_connection();
	for (size_t i = 1; i < pollfds.size(); i++)
	{
		if (pollfds[i].revents == 0) //client hasnt done anything
			continue;
		if (pollfds[i].revents & POLLHUP) //client disconnected
		{
			std::cout << "Socket " << i << " hung up." << std::endl;
			socket_cleanup(pollfds[i].fd);
			// exit(0);
		}
		if (pollfds[i].revents & POLLIN) //client sent server a message
		{
			std::string message;
			message = receive(i);
			create_command(message, get_user(pollfds[i].fd));
		}
		if (pollfds[i].revents & POLLOUT) //client is ready for a response
		{
			respond(get_user(pollfds[i].fd));
		}
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
		std::cout << BLUE << line << RESET << std::endl;
		if (!line.empty()) //might be useless? might not be good enough? shrug
		{
			std::istringstream split(line);
			std::string token;
			while (split >> token)
				command.add_argument(token);
		}
		command.execute();
	}
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */