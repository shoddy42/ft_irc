// #include <Server.hpp>
// #include <iostream>
#include "../../include/Command.hpp"

static bool name_exists(std::string name, Server &server, User &caller)
{
	User &user = server.get_user(name);
	if (user.get_nickname() == name || (user.get_username() == name && &user != &caller))
		return (true);
	return (false);
}

bool is_alnum(const std::string& str) {
    for (char c : str)
        if (!std::isalnum(c))
            return (false);
    return (true);
}
void	Command::nick()
{
	std::string desired_name = _arguments[1];

	if (!is_alnum(desired_name) )
		return;
	if (name_exists(desired_name, _server, _caller))
	{
		std::string reply = std::string(SERVER_SIGNATURE) + " 433 " + _caller.get_nickname() + " " + desired_name + " :Nickname is already in use";
		_caller.add_response(reply);
		return;
	}
	std::string reply = usermask(_caller) + " NICK " + desired_name;
	_caller.add_response(reply);
	_caller.set_nickname(desired_name);
}