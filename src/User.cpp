/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   User.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2024/02/09 16:49:51 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

User::User(int sock): _username(""), _nickname(""), _socket(sock), _authenticated(false)
{

}

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

User &User::operator=(User const &src)
{
	if (this == &src)
		return (*this);
	_responses = src._responses;
	_username = src._username;
	_nickname = src._nickname;
	_socket = src._socket;
	_authenticated = src._authenticated;
	return (*this);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

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

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

// getters
const int	&User::get_socket(void)
{
	return (_socket);
}

const bool	&User::get_authenticated(void)
{
	return(_authenticated);
}

const std::string	&User::get_username(void)
{
	return(_username);
}

const std::string	&User::get_nickname(void)
{
	return(_nickname);
}

// setters
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

/* ************************************************************************** */