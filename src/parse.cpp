#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "../include/Commands.hpp"

void	parse(std::string buffer)
{
	std::stringstream ss(buffer);

	std::string line;
	while (std::getline(ss, line, '\n'))
	{
		std::vector<std::string> command;
		Commands test;
		std::cout << "Line: " << line << std::endl;
		if (!line.empty()) //might be useless? might not be good enough? shrug
		{
			std::istringstream split(line);
			std::string token;
			while (split >> token)
				command.push_back(token);
		}
		test.parseCommand(command);
	}


}
