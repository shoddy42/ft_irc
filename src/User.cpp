/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   User.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2024/03/05 15:44:15 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/User.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

User::User(int sock): _username(NULL_USER), _nickname(NULL_USER), _authenticated(false), _signed_up(false), _socket(sock)
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
	_realname = src._realname;
	_hostname = src._hostname;
	_socket = src._socket;
	_authenticated = src._authenticated;
	_signed_up = src._signed_up;
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

const bool	&User::get_sign_up(void)
{
	return(_signed_up);
}

const std::string	&User::get_username(void)
{
	return(_username);
}

const std::string	&User::get_nickname(void)
{
	return(_nickname);
}

const std::string	&User::get_realname(void)
{
	return(_realname);
}

const std::string	&User::get_hostname(void)
{
	return(_hostname);
}

// setters

void	User::set_socket(int socket)
{
	_socket = socket;
}

void	User::set_nickname(std::string nick)
{
	_nickname = nick;
}

void	User::set_username(std::string username)
{
	_username = username;
}

void	User::set_realname(std::string realname)
{
	_realname = realname;
}

void	User::set_hostname(std::string hostname)
{
	_hostname = hostname;
}

void	User::authenticate(void)
{
	_authenticated = true;
}

void	User::sign_up(void)
{
	_signed_up = true;
}

/* ************************************************************************** */