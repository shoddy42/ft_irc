#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"
#include <vector>

void	Command::privmsg(void)
{
	std::cout << ORANGE << "PRIVMSG called\n" << RESET;

	//todo: figure out what HOSTNAME should actually be
	//create sender prefix
	std::string msg_text = ":" + _caller.get_nickname() + "!" + _caller.get_username() + "@";
	msg_text += HOSTNAME;

	//finish packet, except for CRLF
	for(size_t i = 0; i < _arguments.size(); i++)
		msg_text += " " + _arguments[i];

	//todo:? other channel prefixes??
	if (_arguments[1][0] == '#') //message to channel
	{
		Channel &channel = _server.get_channel(_arguments[1]);

		std::cout << GREEN << "usr list size =  " << channel._user_list.size() << RESET << std::endl;
		channel.send_message(msg_text, _caller);
	}
	else	//message to user
	{
		User	&recipient = _server.get_user(_arguments[1]);
		std::cout << GREEN << "sending privmsg to " << recipient.get_nickname() << RESET << std::endl;

		recipient.add_response(msg_text);
	}
}