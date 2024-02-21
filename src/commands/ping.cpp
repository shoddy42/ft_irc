#include "../../include/Command.hpp"

void	Command::ping(void)
{
	std::string response = "PONG " + std::string(SERVER_SIGNATURE);
	_caller.add_response(response);
}