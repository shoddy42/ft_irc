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
# include <sstream>
# include <queue>
# include <chrono>

# include <stdlib.h>
# include <time.h>
# include <unistd.h>
# include <poll.h>
# include <signal.h>
# include <arpa/inet.h>
# include <sys/socket.h>

// Name the bot will register to the server with, using USER
# define BOT_NAME	"Quantum"
# define BOT_USER	"AdvancedAI"
# define BOT_HOST	"OpenAI"
# define BOT_REAL	"QuantumSuperAI3.5"

# define API_KEY	"" //not functional

	// Variables //
# define BOT_CHANNEL "#general"
# define BOT_CHANNEL_PASS ""
# define JOIN_MESSAGE_WINDOW 2
# define REACTION_TIME	0
# define GPT_COOLDOWN	10

	// Advanced Variables //
# define BOT_IP			"127.0.0.1" //127.0.0.1 for local host.
# define POLL_TIMEOUT	2000
# define BUFFER_SIZE	512

std::string ask_question(const std::string& question);std::string ask_question(const std::string& question); //not functional

class Bot
{
	private:
		std::queue<std::string> _received;
		std::queue<std::string> _responses;

		std::chrono::steady_clock::time_point _join_time;
		std::chrono::steady_clock::time_point _gpt_time;
		std::string _unfinished_packet;
		std::string _password;
		pollfd		_server;

		int	_socket;
		int _port;

		bool _was_kicked;
		bool _was_modded;

		bool _disregarding_notice;
		bool _in_server;
		bool _in_channel;

		std::string mask(void);
		void message_channel(std::string msg);
		std::string generate_text(const std::string& prompt);
		void execute_advanced_ai(std::string);
		void process(std::string packet);
		std::string receive(void);
		void respond(void);
		Bot(void);

	public:
	//	constructors  //
	
		Bot(std::string port, std::string password);
		Bot(const Bot &src);
		Bot &operator=(Bot const &src);
		~Bot(void);

	//	methods  //
	
		void login(void);
		void serve(void);
		void stop(void);

	//	getters/setters  //
};

#endif
