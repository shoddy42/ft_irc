#include "../../include/Command.hpp"

void	Command::quit(void)
{
	std::cout << ORANGE << "QUIT COMMAND CALLED WITH: " << RESET << std::endl;
	_server.delete_user(_caller);
}