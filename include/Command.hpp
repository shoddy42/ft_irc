/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Command.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/05 15:46:16 by shoddy        #+#    #+#                 */
/*   Updated: 2024/03/06 03:45:02 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP
# include "../include/Server.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"

# include <iostream>
# include <string>
# include <vector>
# include <map>

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
		void add_argument(std::string input);
		std::vector<std::string> split_argument(std::string argument);
		bool is_alnum(const std::string &str);

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

		void		mode(void);
		void		mode_user(User &user, std::string flag);
		void		mode_channel(Channel &channel);
		std::string	mode_password(Channel &channel, bool is_plus, std::string password);
		std::string	mode_limit(Channel &channel, bool is_plus, std::string limit);
		std::string	mode_operator(Channel &channel, bool is_plus, User &user);
		std::string	mode_invite(Channel &channel, bool is_plus);
		std::string	mode_topic(Channel &channel, bool is_plus);
		
		class InputException
		{
			public:
				const char *what(void) const throw();
		};

	private:
		Command();
};

#endif