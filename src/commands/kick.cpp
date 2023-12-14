#include "../../include/User.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Command.hpp"

void Command::kick(void)
{
    std::cout << ORANGE << "Kick command called\n" << RESET;
    std::string mem = _arguments[2];
    std::string chan = _arguments[1];
    
    Channel &channel = _server.get_channel(chan);
    User &member = _server.get_user(mem);




    channel.remove_user(member);

}