#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::part(void)
{
    std::cout << ORANGE << "Part command called\n" << RESET;
    Channel &channel = _server.get_channel(_arguments[1]);

    channel.remove_user(_caller);
    // std::string response = SERVER_SIGNATURE;
	// response += " PART " + channel.get_name() + " :You have left the channel " + channel.get_name();
	// _caller.add_response(response);
}