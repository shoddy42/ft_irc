#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

// static std::vector<std::string> split_input(std::string input)
// {
// 	std::vector<std::string>	args;
// 	size_t	pos;
// 	size_t	len;
	
// 	pos = input.find(',');
// 	if (pos == std::string::npos)
// 	{ 
// 		args.push_back(input);
// 		return (args);
// 	}
// 	args.push_back(input.substr(0, pos));
// 	len = pos + 1;
// 	while (len < input.length())
// 	{
// 		pos = input.find(',', len);
// 		if (pos == std::string::npos)
// 		{
// 			args.push_back(input.substr(len, std::string::npos));
// 			return (args);
// 		}
// 		args.push_back(input.substr(len, pos - len));
// 		len = pos + 1;
// 	}
// 	return (args);
// 		// args.push_back(input.substr(input.find_first_not_of(',', pos)))
// }

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