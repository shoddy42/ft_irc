#include "../../include/Command.hpp"

void	Command::ping(void)
{
	std::cout << ORANGE << "Ping called: " << RESET << std::endl;

	std::string response = "PONG ";
	response += SERVER_SIGNATURE;

	_caller.add_response(response);
}