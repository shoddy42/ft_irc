#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"
#include <vector>

/**
 * @brief Throws An Exception, Allowing For Try/Catch Block Interaction
 * 
 * @return const char* 
 */
const char *Command::InputException::what(void) const throw()
{
	return ("this message might be doodoo but your input was too!");
}

/**
 * @brief Adds message To The Server Signature, Sends It To The Client Attempting To Send An Invite & Throws An Exception
 * 
 * @param caller 
 * @param message 
 */
static void	invite_error(User &caller, std::string message)
{
	std::string reply = SERVER_SIGNATURE;
	reply += message;
	caller.add_response(reply);
	throw (Command::InputException());
}

/**
 * @brief Checks The Input For Validity, Sending The Invite To The recipient If All Is Valid & Sending An Error To The caller If Something Is Invalid
 */
void    Command::invite(void)
{
	std::vector<std::string> user_names = split_argument(_arguments[1]);
	std::vector<std::string> channel_names = split_argument(_arguments[2]);

	if (channel_names.size() != 1 && channel_names.size() != user_names.size())
	{
		std::string msg_text = usermask(_caller) + " ERROR : Invite List Mismatch";
		_caller.add_response(msg_text);
		return ;
	}
	for (int i = 0; i < user_names.size(); i++)
	{
		User		&target = _server.get_user(user_names[i].data());
		Channel 	&channel = _server.get_channel(channel_names[0].data());
		if	(channel_names.size() != 1 && i != 0)
			channel = _server.get_channel(channel_names[i].data());
		try
		{
			// grab args
				// check if channel exists
			if (channel.get_name() == NULL_CHANNEL_NAME)
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
			continue; ;
		}
			// invite only shenanigans
		channel.add_invited(target);
			// send recipient an invite (essentially privmsg)
		std::string msg_text = usermask(_caller) + " INVITE ";
		User	&recipient = _server.get_user(_arguments[1]);
		std::cout << PURPLE << "sending invite to " << recipient.get_nickname() << RESET << std::endl;
		msg_text +=  _caller.get_nickname() + " " + _arguments[2];
		recipient.add_response(msg_text);

		std::string reply = SERVER_SIGNATURE;
		reply += " 341 " + _caller.get_nickname() + " " + recipient.get_nickname() + " " + channel.get_name();
		_caller.add_response(reply);
	}
}
