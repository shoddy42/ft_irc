#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"
#include <vector>


const char *Command::InputException::what(void) const throw()
{
	return ("this message might be doodoo but your input was too!");
}


static void	invite_error(User &caller, std::string message)
{
	caller.add_response(message);
	throw (Command::InputException());
}

void    Command::invite(void)
{
	std::cout << ORANGE << "INVITE called\n" << RESET;

	// invite_error(_caller, "Testeroni");
	// try{
	// 	_caller.add_response("Testeroni");
	// 	throw (Command::InputException());
	// }
	// catch (InputException &e){
	// 	return ;
	// }
	try
	{
		// grab args
		Channel 	&channel2join = _server.get_channel(_arguments[2]);
		User		&target2invite = _server.get_user(_arguments[1]);
		// std::cout << "UserName IS " << target2invite.get_username() << "\n";
		// invite_error(_caller, target2invite.get_username());
		// return ;
			// check if channel exists
		if (channel2join.get_name() == "dummy channel")
			invite_error(_caller, "Error: This Channel Does Not Exist");
			// check if recipient exists
		else if (target2invite.get_username() == "")
			invite_error(_caller, "Error: This User Does Not Exist");
			// check if caller is in the channel
		else if (!channel2join.is_user(_caller))
			invite_error(_caller, "Error: You Cannot Send An Invite To A Channel You Aren't In");
			// check if recipient isn't already in the channel
		else if (channel2join.is_user(target2invite))
			invite_error(_caller, "Error: The User You Are Attempting To Invite Is Already In This Channel");
			// check if recipient isn't banned from this channel

			// invite only shenanigans
	}
	catch (InputException &e)
	{
		return ;
	}
		// send recipient an invite (essentially privmsg)
	std::string msg_text = ":" + _caller.get_nickname() + "!" + _caller.get_username() + "@";
	msg_text += HOSTNAME;
	User	&recipient = _server.get_user(_arguments[1]);
	std::cout << GREEN << "sending invite to " << recipient.get_nickname() << RESET << std::endl;
	msg_text += " " + _caller.get_nickname() + " Has Invited You To Join The Following Channel: " + _arguments[2];
	recipient.add_response(msg_text);
}
