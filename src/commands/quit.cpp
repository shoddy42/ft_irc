#include "../../include/Command.hpp"

void	Command::quit(void)
{
	// std::cout << ORANGE << "Quit command called." << RESET << std::endl;
	_server.delete_user(_caller);
}