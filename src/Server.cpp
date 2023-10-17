/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2023/10/18 01:03:49 by root          ########   odam.nl         */
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
#include <iostream> //todo: delete

int Server::add_user(int sock)
{
	int	id = ++last_user_id;

	pollfd new_fd;
	new_fd.fd = sock;
	new_fd.events = POLLIN | POLLOUT;
	poll.push_back(new_fd);

	//todo: think if error handling is needed?
	User joe(sock, poll[id], id);
	users.push_back(joe);
	current_users++;
	return (0);
}

#include <iostream> //todo: delete this

/**
 * @brief Creates a new socket to listen on port. Configures server.poll[0] to be this new socket.
 * 
 * @param port Port to bind socket to
 * @return int (the socket FD)
 */
void	Server::start(int port)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
		error_exit("Failed to create socket. errno: ");

	
    sockaddr_in sock_address;
	sock_address.sin_port = htons(port);
    sock_address.sin_family = AF_INET;
    sock_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr*)&sock_address, sizeof(sock_address)) < 0)
		error_exit("Failed to bind to port. errno: ");

    if (listen(sock, MAX_CLIENTS) < 0)
		error_exit("Failed to listen on socket. errno: ");
	
	pollfd shoe;
	shoe.fd = sock;
	shoe.events = POLLIN;
	poll.push_back(shoe);
}


/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */