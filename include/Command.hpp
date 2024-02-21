#ifndef COMMAND_HPP
# define COMMAND_HPP
# include <iostream>
# include <string>
# include <unordered_map>
# include <map>
# include <vector>
# include "../include/Server.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"

class Server;

class Channel;

class Command
{
	private: 
		typedef void (Command::*Function)(); //command function definition for the function pointer map
		static std::map<std::string, Command::Function> command_map;
		std::vector<std::string> _arguments;
		Server	&_server;
		User	&_caller;

	public:
	//	constructors  //

		Command(Server &server, User &caller);
		~Command(void);
		Command &operator=(Command const &src);

	//	methods  //

		void execute(void);
		void add_argument(std::string argument);

	//	command functions  //

		void	privmsg(void);
		void	invite(void);
		void 	topic(void);
		void	nick(void);
		void	ping(void);
		void	join(void);
		void	user(void);
		void 	kick(void);
		void	pass(void);
		void	quit(void);
		void	part(void);
		void	who(void);

	//	mode functions  //

		void	mode(void);
		void	mode_password(Channel &channel, bool is_plus, std::string password);
		void	mode_limit(Channel &channel, bool is_plus, std::string limit);
		void	mode_operator(Channel &channel, bool is_plus, User &user);
		void	mode_invite(Channel &channel, bool is_plus);
		void	mode_topic(Channel &channel, bool is_plus);
		void	mode_channel(Channel &channel);
		void	mode_user(User &user, std::string flag);
		
		class InputException
		{
			public:
				const char *what(void) const throw();
		};

	private:
		Command();
};

#endif