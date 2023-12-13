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
	for(size_t i = 0; i < arguments.size(); i++)
		msg_text += " " + arguments[i];

	//todo:? other channel prefixes??
	if (arguments[1][0] == '#') //message to channel
	{
		Channel &channel = _server.get_channel(arguments[1]);
		Message message(_caller, msg_text);

		std::cout << GREEN << "usr list size =  " << channel._user_list.size() << RESET << std::endl;
		// channel.add_message(message);
		// channel.send_text(msg_text);
		channel.send_message(message);
	}
	else	//message to user
	{
		Message message(_caller, msg_text);
		User	&recipient = _server.get_user(arguments[1]);
		std::cout << GREEN << "sending privmsg to " << recipient.get_nickname() << RESET << std::endl;


		recipient.add_response(msg_text);
	}
}