#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::kick(void)
{
    std::cout << ORANGE << "Kick command called\n" << RESET;
    std::string mem = _arguments[2];
    std::string chan = _arguments[1];
    std::string reason;
    Channel &channel = _server.get_channel(chan);
    User	&member = _server.get_user(mem);

	//commented out the 482 reply as irssi assumes just because you attempted a command you get kicked for no reason???
	if (channel.is_operator(_caller) == false)
	{
		std::cout << "Unauthorized call for kick\n";
		// std::string reply = SERVER_SIGNATURE;
		// reply += " 482 " + _caller.get_nickname() + " " + channel.get_name() + " :You are not an operator in the channel " + channel.get_name();
		// _caller.add_response(reply);
		return;
	}

    if (_arguments[3].empty() == true)
        reason = "";
    else
        reason = _arguments[3];

    if(channel.is_operator(member) == true)
        return;

    channel.remove_user(member);
	channel.remove_invited(member);
    std::string response = SERVER_SIGNATURE;
	response += "KICK" + chan + _caller.get_username() + " " + reason;
	channel.send_message(response, _caller);
	// channel.send_message()
	_caller.add_response(response);
}