// #include <Server.hpp>
// #include <iostream>
#include "../../include/Command.hpp"

static void printVectorStrings(const std::vector<std::string> &strings) //debug function. todo: remove
{
	for (int i = 0; i < strings.size(); i++)
	{
		std::cout << strings[i].data() << " ";
	}
	std::cout << std::endl;
}

static bool name_exists(std::string name, Server &server)
{
	for (std::list<User>::iterator user = server.users.begin(); user != server.users.end(); user++)
		if (user->get_nickname() == name)
			return (true);
	return (false);
}

void	Command::nick()
{
	std::cout << ORANGE << "NICK COMMAND CALLED WITH: " << RESET << std::endl;

	printVectorStrings(_arguments);

	std::string desired_name = _arguments[1];
	std::cout << "Des name = " << desired_name << std::endl;
	if (name_exists(desired_name, _server))
	{
		std::cout << "Name already exists\n";
	}
	else
	{
		std::string response;

		response = "::" + _caller.get_username() + " NICK " + desired_name;
		std::cout << L_BLUE << response << RESET << std::endl;
		_caller.add_response(response);
		_caller.set_nickname(desired_name);
	}
}