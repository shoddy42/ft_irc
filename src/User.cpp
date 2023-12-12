/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   User.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2023/10/18 01:03:11 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../include/User.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

User::User(int sock, int id): _socket(sock), _id(id)
{

}

User::User(const User &src): _socket(src._socket), _id(src._id)
{
	if (this != &src)
		*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

User::~User(void)
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

//todo: actually make this function, or delete if not needed
User &User::operator=(User const &src)
{
	if (this == &src)
		return (*this);

	return (*this);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const int	User::get_socket(void)
{
	return (_socket);
}

const std::string	User::get_name(void)
{
	return(_name);
}

const std::string	User::get_nick(void)
{
	return(_nick);
}

void	User::set_socket(int socket)
{
	_socket = socket;
}

void	User::set_name(std::string name)
{
	_name = name;
}

void	User::set_nick(std::string nick)
{
	_nick = nick;
}



/* ************************************************************************** */