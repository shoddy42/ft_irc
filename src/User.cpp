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


// User::User(void)
// {
// 	poll_data.fd = sock;
// 	poll_data.events = POLLIN & POLLOUT;
// }

User::User(int sock, pollfd shoe, int id): poll_data(shoe), sock(sock), _id(id)
{

}

User::User(const User &src): poll_data(src.poll_data), sock(src.sock), _id(src._id)
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
	return (*this);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

/*
** --------------------------------- ACCESSOR ---------------------------------
*/


/* ************************************************************************** */