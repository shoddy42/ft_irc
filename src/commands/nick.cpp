// #include <Server.hpp>
// #include <iostream>
#include "../../include/Command.hpp"

static bool name_exists(std::string name, Server &server)
{
	for (std::list<User>::iterator user = server.users.begin(); user != server.users.end(); user++)
		if (user->get_nickname() == name)
			return (true);
	return (false);
}

void	Command::nick()
{
	// std::cout << ORANGE << "NICK command called " << RESET << std::endl;
	std::string desired_name = _arguments[1];

	if (name_exists(desired_name, _server))
	{
		std::cout << "Name already exists\n";
		return;
	}

	std::string reply = ":" + _caller.get_nickname() + "!" + _caller.get_username() + "@";
	reply += HOSTNAME;
	reply += " NICK " + desired_name;
	std::cout << L_BLUE << reply << RESET << std::endl;
	_caller.add_response(reply);
	_caller.set_nickname(desired_name);
}