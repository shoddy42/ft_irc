#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"
#include <vector>

void    Command::invite(void)
{
	std::cout << ORANGE << "INVITE called\n" << RESET;

		// grab args

		// check if channel exists
	
		// check if user is in the channel
		// check if recipient exists
		// check if recipient isn't already in the channel
		// check if recipient isn't banned from this channel
		// check if 

		// invite only shenanigans

		// send recipient an invite (essentially privmsg)
//create sender prefix
	std::string msg_text = ":" + _caller.get_nickname() + "!" + _caller.get_username() + "@";
	msg_text += HOSTNAME;
	User	&recipient = _server.get_user(_arguments[1]);
	std::cout << GREEN << "sending invite to " << recipient.get_nickname() << RESET << std::endl;
	msg_text += " " + _caller.get_nickname() + " Has Invited You To Join The Following Channel(s): ";
	msg_text += _arguments[2];// needs extra conditions for multiple channels?
	recipient.add_response(msg_text);
}
