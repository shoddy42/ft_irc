#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::part(void)
{
    // std::cout << ORANGE << "Part command called\n" << RESET;
    Channel &channel	= _server.get_channel(_arguments[1]);
	std::string reason	= "";
	for (size_t i = 2; i < _arguments.size(); i++)
		reason += _arguments[i] + " ";
	if (channel.is_user(_caller) == false)
	{
		std::string reply = SERVER_SIGNATURE;
		reply += " 442 " + _caller.get_nickname() + " " + _arguments[1] + " :You are not in that channel.";
		_caller.add_response(reply);
	}
    channel.remove_user(_caller, reason);
}