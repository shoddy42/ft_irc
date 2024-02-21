#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::part(void)
{
    Channel &channel	= _server.get_channel(_arguments[1]);
	std::string reason	= " ";
	for (size_t i = 2; i < _arguments.size(); i++)
		reason += _arguments[i] + " ";
	if (channel.is_user(_caller) == false)
	{
		std::string reply = std::string(SERVER_SIGNATURE) + " 442 " + _caller.get_nickname() + " " + channel.get_name() + " :You are not in " + channel.get_name();
		// _caller.add_response(ERR_NOT_IN_CHANNEL(_caller.get_nickname(), _arguments[1]));
		_caller.add_response(reply);
		return;
	}
    channel.remove_user(_caller, reason);
}