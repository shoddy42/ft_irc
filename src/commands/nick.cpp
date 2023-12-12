// #include <Server.hpp>
// #include <iostream>
#include "../../include/Command.hpp"

static bool name_exists(std::string name, Server &server)
{
	for (std::list<User>::iterator it = server.users.begin(); it != server.users.end(); it++)
		if (it->get_name() == name)
			return (true);
	return (false);
}

int Command::nick()
{
	std::cout << ORANGE << "NICK COMMAND CALLED WITH: " << RESET << std::endl;

	// printVectorStrings(command);
	// server.users.
	if (name_exists(arguments[0], _server))
	{
	}
	return (0);
}