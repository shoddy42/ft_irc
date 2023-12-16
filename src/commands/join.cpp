#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

//todo: implement check to see if channel name is valid according to RFC 2812
void	Command::join(void)
{
	std::cout << ORANGE << "Join called" << RESET << "\n";
	std::string requested_channel = _arguments[1];

	Channel &channel = _server.get_channel(requested_channel);
	//todo: filter if user is already in channel
	if (channel.get_name() == requested_channel) //channel exists already
	{
		std::cout << "Channel found " << channel.get_name() << std::endl; 

		std::cout << GREEN << "usr list size =  " << channel._user_list.size() << RESET << std::endl;
		channel.add_user(_caller);
		std::cout << GREEN << "usr list size =  " << channel._user_list.size() << RESET << std::endl;
	}
	else // channel does not exist
	{
		std::cout << "Creating channel " << requested_channel << "\n";
		Channel new_channel(requested_channel, _server);
		new_channel.add_user(_caller);
		new_channel.add_operator(_caller);
		_server.channels.push_back(new_channel);
	}
	// std::string response = SERVER_SIGNATURE;
	// response += " 332 " + _caller.get_username() + " " + requested_channel + " :Welcome to the " + requested_channel + " channel.";
	// _caller.add_response(response);
}