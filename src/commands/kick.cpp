#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::kick(void)
{
    std::cout << ORANGE << "Kick command called\n" << RESET;
    std::string mem = _arguments[2];
    std::string chan = _arguments[1];
    std::string reason;

    if (_arguments[3].empty() == true)
        reason = "";
    else
        reason = _arguments[3];

    Channel &channel = _server.get_channel(chan);
    User &member = _server.get_user(mem);

    if(channel.is_operator(member) == true)
        return;

    channel.remove_user(member);
    std::string response = SERVER_SIGNATURE;
	response += "KICK" + chan + _caller.get_username() + " " + reason;
	_caller.add_response(response);
}