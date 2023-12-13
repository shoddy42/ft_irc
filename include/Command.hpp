#ifndef COMMAND_HPP
# define COMMAND_HPP
# include <iostream>
# include <string>
# include <unordered_map>
# include <map>
# include <vector>
# include "../include/Server.hpp"
# include "../include/User.hpp"

//todo: think about whether this even needs to be a class or not. seems like it would be neater if not?

class Server;

class Command
{
	public:
		Command(Server &server, User &caller); //todo: change caller to reference
		~Command(void);
		Command &operator=(Command const &src);

		void execute(void);
		void add_argument(std::string argument);

		typedef void (Command::*CommandFunction)(); //command function definition for the jump map

    //command functions
		// void	call_(std::string key);
		void	nick(void);
		void	ping(void);
		void	join(void);
		void	privmsg(void);

		
	private:
		std::vector<std::string> arguments;
		Server	&_server;
		User	&_caller;
		static std::map<std::string, Command::CommandFunction> command_map;
		// Command();

};

#endif
//invite nick kick topic mode pass ban kill list me unban