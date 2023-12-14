/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Message.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2023/09/05 18:43:58 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../include/Message.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/


// Message::Message(void)
// {
// }

Message::Message(User &sender, std::string &text): _sender(sender), _text(text)
{
}

Message::Message(const Message &src): _sender(src._sender), _text(src._text)
{
	if (this != &src)
		*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Message::~Message(void)
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Message &Message::operator=(Message const &src)
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

const std::string &Message::get_text()
{
	return (_text);
}

const User	&Message::get_sender(void)
{
	return (_sender);
}


/* ************************************************************************** */