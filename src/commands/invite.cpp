/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   invite.cpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/05 15:06:19 by shoddy        #+#    #+#                 */
/*   Updated: 2024/03/05 15:45:33 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

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

	if (channel_names.size() != 1 && user_names.size() != 1 && channel_names.size() != user_names.size())
	{
		std::string msg_text = usermask(_caller) + " ERROR :Invite List Mismatch";
		_caller.add_response(msg_text);
		return ;
	}
	for (size_t i = 0; i < user_names.size() || i < channel_names.size(); i++)
	{
		std::string user_name = user_names[0];
		if (user_names.size() != 1 && user_names[0] != user_names[i])
			user_name = user_names[i];
		std::string channel_name = channel_names[0];
		if	(channel_names.size() != 1 && channel_names[0] != channel_names[i])
			channel_name = channel_names[i];
		User		*target = &_server.get_user(user_name);
		Channel 	*channel = &_server.get_channel(channel_name);

		try
		{
				// check if channel exists
			if (channel->get_name() == NULL_CHANNEL_NAME)
				invite_error(_caller, " 403 " + channel_name + " :No such channel");
				// check if recipient exists
			else if (target->get_username() == "")
				invite_error(_caller, " 401 " + user_name + " :No such nick/channel");
				// check if caller is in the channel
			else if (!channel->is_user(_caller))
				invite_error(_caller, " 442 " + channel_name + " :You're not on that channel");
				// check if caller is an operator
			else if (channel->is_operator(_caller) == false)
				invite_error(_caller, " 482 " + channel_name + " :You're not channel operator");
				// check if recipient isn't already in the channel
			else if (channel->is_user(*target))
				invite_error(_caller, " 443 " + channel_name + " " + target->get_username() + " :User already on channel");
		}
		catch (InputException &e)
		{
			continue;
		}
		if (channel->is_invited(*target))
			return;
		channel->add_invited(*target);
		std::cout << PURPLE << "sending invite to " << target->get_nickname() << RESET << std::endl;
		std::string msg_text = usermask(_caller) + " INVITE " + _caller.get_nickname() + " " + channel_name;
		target->add_response(msg_text);

		std::string reply = SERVER_SIGNATURE;
		reply += " 341 " + _caller.get_nickname() + " " + target->get_nickname() + " " + channel->get_name();
		_caller.add_response(reply);
	}
}
