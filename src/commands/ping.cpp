#include "../../include/Command.hpp"

void	Command::ping(void)
{
	std::cout << ORANGE << "PING COMMAND CALLED WITH: " << RESET << std::endl;

	std::string response = "PONG ";
	response += SERVER_SIGNATURE;

	_caller.add_response(response);
}