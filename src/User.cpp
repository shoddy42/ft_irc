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

User::User(int sock): _socket(sock), _username(""), _nickname(""), _authenticated(false)
{

}

//todo: actually make copy constru or delete?
User::User(const User &src): _socket(src._socket)
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

const int	&User::get_socket(void)
{
	return (_socket);
}

const std::string	&User::get_username(void)
{
	return(_username);
}

const std::string	&User::get_nickname(void)
{
	return(_nickname);
}

const bool	&User::get_authenticated(void)
{
	return(_authenticated);
}


void	User::set_socket(int socket)
{
	_socket = socket;
}

void	User::set_username(std::string name)
{
	_username = name;
}

void	User::set_nickname(std::string nick)
{
	_nickname = nick;
}

void	User::authenticate(void)
{
	_authenticated = true;
}


void	User::add_response(std::string response)
{
	_responses.push(response);
}

std::string	User::give_response(void)
{
	std::string response;

	if (_responses.empty())
		return ("");
	response = _responses.front();
	_responses.pop();
	return (response);
}

int		User::check_response(void)
{
	return (_responses.size());
}


/* ************************************************************************** */