#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::kick(void)
{
	std::cout << ORANGE << "Kick command called\n" << RESET;
	std::string mem = _arguments[2];
	std::string chan = _arguments[1];
	std::string reason = "";
	for (size_t i = 3; i < _arguments.size(); i++)
		reason += _arguments[i] + " ";
	
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

	if (channel.is_user(member) == false)
		return;
	// if (channel.is_operator(member) == true)
	// 	return;


	std::string channel_response = ":" + _caller.get_nickname() + "!" + _caller.get_username() + "@";
	channel_response += HOSTNAME;
	channel_response += " KICK " + channel.get_name() + " " + member.get_nickname() + " " + reason;
	//todo: maybe create send_notice, which is just send message but instead it doesnt get stored in history to bypass irssi's leave?
	// channel.send_message(channel_response, _caller);
	channel.send_notice(channel_response, _caller);

	// std::string response = SERVER_SIGNATURE;
	// std::cout << "REASON IS: " << reason << std::endl;
	// response += " KICK " + channel.get_name() + " " + member.get_nickname() + " " + reason;
	// member.add_response(response);

	channel.kick_user(member);
	// channel.remove_user(member, reason);
	// channel.remove_invited(member);
}