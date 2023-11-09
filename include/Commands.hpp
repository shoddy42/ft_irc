#ifndef COMMANDS_HPP
# define COMMANDS_HPP
#include <iostream>
#include <string>


class Commands
{
	public:
		Commands();
		~Commands(void);
		Commands &operator=(Commands const &src);
        void parseCommand(std::string CommandStr);
    //command functions

		
	private:
		std::string ircCommands[11];

};

//invite nick kick topic mode pass ban kill list me unban