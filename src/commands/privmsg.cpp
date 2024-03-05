#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"
#include <vector>

void	Command::privmsg(void)
{
	std::string msg_text = usermask(_caller);

	//finish packet, except for CRLF
	for(size_t i = 0; i < _arguments.size(); i++)
		msg_text += " " + _arguments[i];

	std::string recipient = _arguments[1];
	if (recipient[0] == '#' || recipient[0] == '+' || recipient[0] == '&' || recipient[0] == '!') //message to channel
	{
		Channel &channel = _server.get_channel(_arguments[1]);
		if (channel.get_name() == NULL_CHANNEL_NAME)
		{
				std::string reply = SERVER_SIGNATURE;
				reply += " 442 " + _caller.get_nickname() + " " + _arguments[1] + " :You are not in the channel " + _arguments[1];
				_caller.add_response(reply);
				return;
		}
		channel.send_message(msg_text, _caller);
	}
	else	//message to user
	{
		User	&recipient = _server.get_user(_arguments[1]);
		recipient.add_response(msg_text);
	}
}