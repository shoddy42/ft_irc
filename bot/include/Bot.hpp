/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Bot.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2024/03/05 17:10:23 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOT_HPP
# define BOT_HPP

# include "../include/colours.hpp"

# include <iostream>
# include <string>
# include <cstring>
# include <queue>

# include <stdlib.h>     /* srand, rand */
# include <time.h>       /* time */
# include <unistd.h>
# include <poll.h>
# include <signal.h>
# include <arpa/inet.h>
# include <sys/socket.h>

# define DEFAULT_NAME	"FT_IRC_BOT"
# define DEFAULT_IP		"127.0.0.1"
# define DEFAULT_CHANNEL "#general"
# define DEFAULT_CHANNEL_PASS ""
# define POLL_TIMEOUT	2000
# define BUFFER_SIZE	512

int guard(int n, std::string error_msg);

class Bot
{
	private:
		std::queue<std::string> _received;
		std::queue<std::string> _responses;

		std::string _unfinished_packet;
		std::string _password;
		std::string	_botname;
		pollfd		_server;

		int	_socket;
		int _port;

		bool _in_server;
		bool _in_channel;

	public:
	//	constructors  //
	
		Bot(std::string port, std::string password);
		Bot(const Bot &src);
		Bot &operator=(Bot const &src);
		~Bot(void);

	//	methods  //
	
		void start(std::string ip);
		void stop(void);
		void serve(void);
		void respond(void);
		std::string receive(void);

		void message_channel(std::string msg);
		void login(void);
		void process(std::string packet);

	//	getters/setters  //
	
	private:
		Bot(void);
};

#endif
