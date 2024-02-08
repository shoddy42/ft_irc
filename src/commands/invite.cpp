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
	std::string reply = SERVER_SIGNATURE;
	reply += message;
	caller.add_response(reply);
	throw (Command::InputException());
}

void    Command::invite(void)
{
	std::cout << ORANGE << "INVITE called\n" << RESET;

	Channel 	&channel = _server.get_channel(_arguments[2]);
	User		&target = _server.get_user(_arguments[1]);
	try
	{
		// grab args
			// check if channel exists
		if (channel.get_name() == "dummy channel")
			invite_error(_caller, " 403 " + _arguments[2] + " :No such channel");
			// check if recipient exists
		else if (target.get_username() == "")
			invite_error(_caller, " 401 " + _arguments[1] + " :No such nick/channel");
			// check if caller is in the channel
		else if (!channel.is_user(_caller))
			invite_error(_caller, " 442 " + _arguments[2] + " :You're not on that channel");
			// check if caller is an operator
		else if (channel.is_operator(_caller) == false)
			invite_error(_caller, " 482 " + _arguments[2] + " :You're not channel operator");
			// check if recipient isn't already in the channel
		else if (channel.is_user(target))
			invite_error(_caller, " 443 " + _arguments[2] + " " + target.get_username() + " :is already on channel");
	}
	catch (InputException &e)
	{
		return ;
	}
		// invite only shenanigans
	channel.add_invited(target);
		// send recipient an invite (essentially privmsg)
	std::string msg_text = ":" + _caller.get_nickname() + "!" + _caller.get_username() + "@";
	msg_text += HOSTNAME;
	msg_text += " INVITE ";
	User	&recipient = _server.get_user(_arguments[1]);
	std::cout << GREEN << "sending invite to " << recipient.get_nickname() << RESET << std::endl;
	msg_text += " " + _caller.get_nickname() + " " + _arguments[2];
	recipient.add_response(msg_text);
}
