/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   nick.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/05 15:05:41 by shoddy        #+#    #+#                 */
/*   Updated: 2024/05/14 18:38:46 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

static bool name_exists(std::string name, Server &server, User &caller)
{
	User &user = server.get_user(name);
	if (&user == &caller || user.get_username() == NULL_USER)
		return (false);
	if (user.get_nickname() == name || (user.get_username() == name))
		return (true);
	return (false);
}

void	Command::nick()
{
	std::string desired_name = limit_name_length(_arguments[1]);
	
	if (desired_name.length() > 1 && desired_name[0] == ':')
		desired_name = desired_name.erase(':');
	if (!is_alnum(desired_name) || desired_name.empty() || _caller.get_nickname().empty())
		return;
	if (name_exists(desired_name, _server, _caller))
	{
		std::string reply = std::string(SERVER_SIGNATURE) + " 433 " + _caller.get_nickname() + " " + desired_name + " :Nickname is already in use";
		_caller.add_response(reply);
		return;
	}
	std::string reply = usermask(_caller) + " NICK " + desired_name;
	_caller.add_response(reply);
	_caller.set_nickname(desired_name);
}