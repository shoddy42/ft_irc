#ifndef COMMANDS_HPP
# define COMMANDS_HPP
#include <iostream>
#include <string>
#include <unordered_map>

//todo: think about whether this even needs to be a class or not. seems like it would be neater if not?

class Commands
{
	public:
		Commands();
		~Commands(void);
		Commands &operator=(Commands const &src);
        void parseCommand(std::vector<std::string> command);
    //command functions

		
	private:
		// std::string ircCommands[11];
		static std::unordered_map<std::string, int(*)(std::vector<std::string> command)> commandMap;

};

#endif
//invite nick kick topic mode pass ban kill list me unban