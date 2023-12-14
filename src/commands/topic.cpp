#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::topic(void)
{
    std::string chan = _arguments[1];
    std::string topic = _arguments[2];

    Channel &channel = _server.get_channel(chan);

    if (channel.is_operator(_caller) == true)
    {
        channel.set_topic(topic);
        std::cout << ORANGE << "called topic and set to " << topic << RESET << "\n"; 
    }
}