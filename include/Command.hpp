#ifndef COMMAND_HPP
# define COMMAND_HPP
# include <iostream>
# include <string>
# include <unordered_map>
# include <map>
# include <vector>
# include "../include/Server.hpp"
# include "../include/User.hpp"

class Server;

class Command
{
	public:
		Command(Server &server, User &caller);
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
		void	user(void);
		void 	kick(void);
		void	pass(void);
		void 	topic(void);
		void	quit(void);


		
	private:
		std::vector<std::string> _arguments;
		Server	&_server;
		User	&_caller;
		static std::map<std::string, Command::CommandFunction> command_map;
		// Command();

};

#endif
//invite nick kick topic mode pass ban kill list me unban