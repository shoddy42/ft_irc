#ifndef REPLY_HPP
# define REPLY_HPP
# include "../include/Server.hpp"

# define RPL_WHOREPLY(channel_name, username, nick, ircoper, chanoper, realname) \
	(":" + std::string(HOSTNAME) + " 352 " + channel_name + " " + username + " " + std::string(HOSTNAME) + " " + std::string(SERVER_SIGNATURE) + " " + nick + " H" + ircoper \
	+ chanoper + " :0 " + realname)



#endif
