/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2023/10/18 11:37:40 by root          ########   odam.nl         */
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

/**
 * @brief Creates a new pollfd, configures it to check for POLLIN and POLLOUT events. Then creates a new user with this data
 * 
 * @param sock 
 * @return int 
 */
int Server::add_user(int sock)
{
	int	id = ++last_user_id;

	pollfd new_fd;
	new_fd.fd = sock;
	new_fd.events = POLLIN | POLLOUT;
	new_fd.revents = 0;
	poll.push_back(new_fd);

	//todo: think if error handling is needed?
	User joe(sock, poll[id], id);
	users.push_back(joe);
	current_users++;
	return (0);
}

/**
 * @brief Creates a new socket to listen on port. Configures server.poll[0] to be this new socket.
 * 
 * @param port Port to bind socket to
 * @return int (the socket FD)
 */
void	Server::start(int port)
{
	sock = guard(socket(AF_INET, SOCK_STREAM, 0), "Failed to create socket. errno:");
	int flags = guard(fcntl(sock, F_GETFL), "Fcntl failed to get flags. errno: ");
	guard(fcntl(sock, F_SETFL, flags | O_NONBLOCK), "Failed to set socket to non-blocking. errno: ");
	
    sockaddr_in sock_address;
	sock_address.sin_port = htons(port);
    sock_address.sin_family = AF_INET;
    sock_address.sin_addr.s_addr = INADDR_ANY;

    guard(bind(sock, (struct sockaddr*)&sock_address, sizeof(sock_address)), "failed to bind to port. errno: ");
	guard(listen(sock, MAX_CLIENTS), "Failed to listen on socket. errno: ");
   
   
    // if (bind(sock, (struct sockaddr*)&sock_address, sizeof(sock_address)) < 0)
	// 	error_exit("Failed to bind to port. errno: ");

    // if (listen(sock, MAX_CLIENTS) < 0)
	// 	error_exit("Failed to listen on socket. errno: ");
	
	pollfd shoe;
	shoe.fd = sock;
	shoe.events = POLLIN;
	poll.push_back(shoe);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */