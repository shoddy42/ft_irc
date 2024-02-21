/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   who.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/21 10:07:06 by shoddy        #+#    #+#                 */
/*   Updated: 2024/02/21 11:27:52 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include "../../include/User.hpp"
// #include "../../include/Channel.hpp"
#include "../../include/Command.hpp"



void	Command::who(void)
{
	std::cout << ORANGE << "Who command called\n" << RESET;
	if (_arguments.size() < 2)
		return; //who whole server possibly
	std::cout << "TEST: " << " s: " << _arguments.size() << "\n";
	std::string channel_name = _arguments[1];
	Channel &channel = _server.get_channel(channel_name);

	if (channel.get_name() == "dummy channel")
	{
		std::cout << RED << "SOMEHOW RETURNED DUMMY CHANNEL IN WHO\n" << RESET;
		return;
	}

	//send end of WHO list for irssi sync
	std::string who_reply = SERVER_SIGNATURE;
	who_reply += " 315 " + _caller.get_nickname() + " " + channel.get_name() + " :End of /WHO list";
	_caller.add_response(who_reply);

}