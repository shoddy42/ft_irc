#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

//todo: split arg 1 and 2 on , to create channel_list and pass_list?
void	Command::join(void)
{
	std::string requested_channel = _arguments[1];
	std::string key = "";
	if (_arguments.size() > 2)
		key = _arguments[2];

	Channel &channel = _server.get_channel(requested_channel);

	if (channel.get_name() != requested_channel) //channel doesn't exist yet, so we create it.
	{
		_server.add_channel(requested_channel, _caller);
		return;
	}

	//make sure user is permitted to join channel
	if (channel.is_user(_caller)) //user is already a member of the channel
		return;
	if (channel.is_invited(_caller) == false) //user is not invited
	{
		std::string reply = std::string(SERVER_SIGNATURE) + " 473 " + _caller.get_nickname() + " " + channel.get_name() + " :Channel is invite only";
		// _caller.add_response(ERR_INVITE_ONLY_CHANNEL(_caller.get_nickname(), channel.get_name()));
		_caller.add_response(reply);
		return;
	}
	if (channel.has_password() == true && key != channel.get_password())
	{
		std::string reply = SERVER_SIGNATURE;
		reply += " 475 " + _caller.get_nickname() + " " + channel.get_name() + " :Wrong key";
		_caller.add_response(reply);
		return;
	}
	//user succesfully joins channel!
	std::string reply = usermask(_caller) + " JOIN :" + channel.get_name();
	channel.add_user(_caller);
	channel.send_notice(reply);
}