#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

//todo: split arg 1 and 2 on , to create channel_list and pass_list?
void	Command::join(void)
{
	std::cout << ORANGE << "Join called" << RESET << "\n";
	std::string requested_channel = _arguments[1];
	std::string key;
	if (_arguments.size() > 2)
		key = _arguments[2];

	Channel &channel = _server.get_channel(requested_channel);

	if (channel.get_name() != requested_channel) //channel doesn't exist yet, so we create it.
	{
		std::cout << "Creating channel " << requested_channel << "\n";
		Channel new_channel(requested_channel, _server);
		new_channel.add_operator(_caller);
		new_channel.add_user(_caller);
		new_channel.add_invited(_caller);
		_server.channels.push_back(new_channel);
		return;
	}

	std::cout << "Channel found " << channel.get_name() << std::endl;
	//make sure user is permitted to join channel
	if (channel.is_user(_caller))
		return;
	if (channel.is_invited(_caller) == false)
	{
		std::string reply = SERVER_SIGNATURE;
		reply += " 473 " + _caller.get_nickname() + " " + channel.get_name() + " :Channel is invite only";
		_caller.add_response(reply);
	}
	if (channel.has_password() == true)
	{
		std::cout << "Password required for " << channel.get_name() << std::endl;
		if (key != channel.get_password())
		{
			std::cout << "Wrong password" << std::endl; 
			std::string reply = SERVER_SIGNATURE;
			reply += " 475 " + _caller.get_nickname() + " " + channel.get_name() + " :Wrong key";
			_caller.add_response(reply);
			return;
		}
	}
	//user succesfully joins channel!
	std::string reply = ":" + _caller.get_nickname() + "!" + _caller.get_username() + "@";
	reply += HOSTNAME;
	reply += " JOIN :" + channel.get_name();
	
	channel.add_user(_caller);
	channel.send_message(reply, _caller);
}