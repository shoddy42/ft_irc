#include "../../include/Command.hpp"

static void parse_arguments(std::string args)
{
	static std::string modes = "itko";

	if (args.empty() || args.length() > 2)
		return; //or error
	if(!args[0] != '-' || args[0] != '+')
	{
		bool found = false;
		for(int i = 0; i < 3; i++)
		{
			if (args[1] == modes[i])
				found = true;
		}
		if (found == false)
			std::cout << "Bad" << std::endl;
			//error or something
	}

}

void	Command::mode(void)
{
	std::cout << ORANGE << "MODE called with: [" << _arguments[1] << "]\n" << RESET;

}

