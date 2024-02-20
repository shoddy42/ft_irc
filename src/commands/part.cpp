#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::part(void)
{
    std::cout << ORANGE << "Part command called\n" << RESET;
    Channel &channel	= _server.get_channel(_arguments[1]);
	std::string reason	= "";
	for (size_t i = 2; i < _arguments.size(); i++)
		reason += _arguments[i] + " ";
    channel.remove_user(_caller, reason);
}