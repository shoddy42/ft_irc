/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   topic.cpp                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/05 15:05:06 by shoddy        #+#    #+#                 */
/*   Updated: 2024/03/05 15:05:06 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

void Command::topic(void)
{
    std::string chan = _arguments[1];
    std::string topic = _arguments[2];
    for (size_t i = 3; i < _arguments.size(); i++)
		topic += " " + _arguments[i];

    Channel &channel = _server.get_channel(chan);

    if (channel.is_topic_restricted() == true && channel.is_operator(_caller) == false)
	{
		std::string reply = std::string(SERVER_SIGNATURE) + " 482 " + channel.get_name() + " :You're not channel operator";
		_caller.add_response(reply);
		return;
	}

    channel.set_topic(topic);
	std::string reply = usermask(_caller) + " TOPIC " + channel.get_name() + " " + topic;
	channel.send_notice(reply);
}