#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::topic(void)
{
    std::string chan = _arguments[1];
    std::string topic = _arguments[2];
    for (size_t i = 3; i < _arguments.size(); i++)
		topic += " " + _arguments[i];

    Channel &channel = _server.get_channel(chan);

    if (channel.is_topic_restricted() == true)
        if (channel.is_operator(_caller) == false)
            return;
    channel.set_topic(topic);
    // std::cout << ORANGE << "topic set to " << topic << RESET << "\n"; 
	std::string reply = usermask(_caller) + " TOPIC " + channel.get_name() + " " + topic;
	channel.send_notice(reply);
}