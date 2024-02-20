#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::part(void)
{
    std::cout << ORANGE << "Part command called\n" << RESET;
    Channel &channel	= _server.get_channel(_arguments[1]);
	std::string reason	= _arguments[2];

    channel.remove_user(_caller, reason);
}