#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::part(void)
{
	std::vector<std::string> channel_names = split_argument(_arguments[1]);

	for (size_t i = 0; i < channel_names.size(); i++)
	{
		// Channel &channel	= _server.get_channel(_arguments[1]);
		Channel &channel	= _server.get_channel(channel_names[i].data());
		std::string reason	= " ";
		for (size_t i = 2; i < _arguments.size(); i++)
			reason += _arguments[i] + " ";
		if (channel.is_user(_caller) == false)
		{
			std::string reply = std::string(SERVER_SIGNATURE) + " 442 " + _caller.get_nickname() + " " + channel_names[i] + " :You are not in " + channel_names[i];
			// _caller.add_response(ERR_NOT_IN_CHANNEL(_caller.get_nickname(), _arguments[1]));
			_caller.add_response(reply);
			// return;
			continue;
		}
		channel.remove_user(_caller, reason);
	}
}