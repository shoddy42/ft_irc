#include "../../include/Command.hpp"

void	Command::quit(void)
{
	_server.remove_user(_caller);
}