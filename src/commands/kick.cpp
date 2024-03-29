/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   kick.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/05 15:05:54 by shoddy        #+#    #+#                 */
/*   Updated: 2024/03/05 15:45:23 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

void Command::kick(void)
{
	std::string mem = _arguments[2];
	std::string chan = _arguments[1];
	std::string reason;
	for (size_t i = 3; i < _arguments.size() && _arguments[i].empty() == false; i++)
		reason += _arguments[i] + (i < _arguments.size() -1 ? " " : "");

	Channel &channel = _server.get_channel(chan);
	User	&member = _server.get_user(mem);

	if (channel.is_operator(_caller) == false)
	{
		std::cout << "Unauthorized call for kick\n";
		std::string reply = SERVER_SIGNATURE;
		reply += " 482 " + _caller.get_nickname() + " " + channel.get_name() + " :You are not an operator in the channel " + channel.get_name();
		_caller.add_response(reply);
		return;
	}

	if (channel.is_user(member) == false)
		return;

	std::string channel_response = usermask(member);
	channel_response += " KICK " + channel.get_name() + " " + member.get_nickname() + " " + reason;
	channel.send_notice(channel_response);
	channel.kick_user(member);
}