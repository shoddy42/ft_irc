#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void	Command::join(void)
{
	std::vector<std::string> channel_names = split_argument(_arguments[1]);
	std::vector<std::string> channel_passwords;
	if (_arguments.size() > 2)
		channel_passwords = split_argument(_arguments[2]);

	for (size_t i = 0; i < channel_names.size(); i++)
	{
		std::string requested_channel = channel_names[i].data();
		std::string key = "";
		if (i < channel_passwords.size())
			key = channel_passwords[i].data();

		Channel &channel = _server.get_channel(requested_channel);

		if (channel.get_name() != requested_channel) //channel doesn't exist yet, so we create it.
		{
			_server.add_channel(requested_channel, _caller);
			continue;
		}

		//make sure user is permitted to join channel
		if (channel.is_user(_caller)) //user is already a member of the channel
			continue;
		if (channel.is_invited(_caller) == false) //user is not invited
		{
			std::string reply = std::string(SERVER_SIGNATURE) + " 473 " + _caller.get_nickname() + " " + channel.get_name() + " :Channel is invite only";
			_caller.add_response(reply);
			continue;
		}
		if (channel.has_password() == true && key != channel.get_password())
		{
			std::string reply = SERVER_SIGNATURE;
			reply += " 475 " + _caller.get_nickname() + " " + channel.get_name() + " :Wrong key";
			_caller.add_response(reply);
			continue;
		}
		//user succesfully joins channel!
		std::string reply = usermask(_caller) + " JOIN :" + channel.get_name();
		channel.add_user(_caller);
		channel.send_notice(reply);
	}
}