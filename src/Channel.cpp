/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Channel.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2023/09/05 18:43:58 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */


#include "../include/Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/


// Channel::Channel(void): _name()
// {
// }

Channel::Channel(std::string channel_name, Server &server): _name(channel_name), _server(server)
{

}


Channel::Channel(const Channel &src): _name(src._name), _server(src._server)
{
	if (this != &src)
		*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel(void)
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Channel &Channel::operator=(Channel const &src)
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


const std::string	&Channel::get_name(void)
{
	return (_name);
}

void	Channel::add_user(User &user)
{
	_user_list.push_back(&user);
}

void	Channel::add_operator(User &user)
{
	_operator_list.push_back(&user);
}

void	Channel::add_message(Message &message)
{
	_message_log.push_back(message);
}

void	Channel::send_text(std::string text)
{
	std::cout << GREEN << "usr list size =  " << _user_list.size() << RESET << std::endl;
	for (std::vector<User *>::iterator user = _user_list.begin(); user != _user_list.end() ; user++)
	{
		//todo: dont send to sender
		std::cout << GREEN << "added response to " << (*user)->get_name() << RESET << std::endl;
		User &real_user = _server.get_user((*user)->get_name());
		(*user)->add_response(text);
	}
	
}



/* ************************************************************************** */