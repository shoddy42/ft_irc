#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"
#include <vector>

static void print_recipients(std::vector<User> &users, std::vector<Channel> &channels) //todo: remove when not needed, just a debug function
{
	std::cout << "Users receiving message: ";
	for (size_t i = 0; i < users.size(); i++)
		std::cout << users[i].get_name() << " ";
	std::cout << std::endl;

	std::cout << "Channels receiving message: ";
	for (size_t i = 0; i < channels.size(); i++)
		std::cout << channels[i].get_name() << " ";
	std::cout << std::endl;
}

static std::vector<User> get_recipient_users(const std::vector<std::string> &arguments, Server &server)
{
	std::vector<User> recipients;
	for (size_t i = 1; i < arguments.size(); i++)
	{
		std::string str = arguments[i];
		//todo: add the other symbols a channel can start with, or just make a #define "#&@!%@@" or whatever the channel prefixes are
		if (str[0] != ':' && str[0] != '#')
		{
			//todo: check its not dummy user?
			recipients.push_back(server.get_user(str));
		}
	}
	return (recipients);
}

static std::vector<Channel> get_recipient_channels(const std::vector<std::string> &arguments, Server &server)
{
	std::vector<Channel> recipients;
	std::cout << PURPLE << "DEBUGGING REC CHAN\n" << RESET;
	for (size_t i = 1; i < arguments.size(); i++)
	{
		std::string str = arguments[i];
		//todo: add the other symbols a channel can start with, or just make a #define "#&@!%@@" or whatever the channel prefixes are
		if (str[0] == '#' || str[0] == '#')
		{
			//todo: check its not dummy channel?
			Channel &recipient = server.get_channel(str);
			std::cout << "added user: " << recipient.get_name() << "\n";
			recipients.push_back(recipient);
		}
	}
	return (recipients);
}

// bad function that stripped all before the : off the message, turns out its not needed, can just send the whole thing xdd

// static std::string get_message(const std::vector<std::string> &arguments)
// {
// 	size_t i = 1;
// 	std::string message;

// 	while(++i < arguments.size())
// 	{
// 		std::string text = arguments[i];
// 		if (text[0] == ':')
// 			break;
// 	}
// 	if (i > arguments.size())
// 		return (message);
// 	message = arguments[i].c_str() + 1;
// 	while(++i < arguments.size())
// 		message += " " + arguments[i];
// 	std::cout << "Msg Test: " << message << std::endl;
// 	return (message);
// }


//todo: figure out rules for strange cases such as: empty message, no recipients, etc?
void	Command::privmsg(void)
{
	std::cout << ORANGE << "PRIVMSG called\n" << RESET;

	// std::vector<Channel> channel_recipients = get_recipient_channels(arguments, _server);
	std::vector<User>	 user_recipients = get_recipient_users(arguments, _server);
	// std::string msg_text = get_message(arguments);

	Channel &channel = _server.get_channel(arguments[1]);

	std::string msg_text = ":" + _caller.get_nick() + "!" + _caller.get_name() + "@";
	//todo: figure out what HOSTNAME should actually be
	msg_text += HOSTNAME;
	for(size_t i = 0; i < arguments.size(); i++)
		msg_text += " " + arguments[i];

	// print_recipients(user_recipients, channel_recipients); //debug function

	//Send message to all channel recipients
	std::cout << GREEN << "usr list size =  " << channel._user_list.size() << RESET << std::endl;
	Message message(_caller, msg_text);
	channel.add_message(message);
	channel.send_text(msg_text);

	

	// for (std::vector<Channel>::iterator channel = channel_recipients.begin(); channel != channel_recipients.end(); channel++)
	// {
	// 	channel->add_message(message);
	// 	channel->send_text(msg_text);
	// }

	//Send message to all users recipients
	
	// if ( == "#")
}