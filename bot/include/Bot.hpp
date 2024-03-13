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

#ifndef Bot_HPP
# define Bot_HPP

# include "../include/colours.hpp"

# include <iostream>
# include <string>
# include <cstring>

# include <unistd.h>
# include <poll.h>
# include <signal.h>
# include <arpa/inet.h>
# include <sys/socket.h>

# define DEFAULT_IP		"127.0.0.1"
# define DEFAULT_NAME	"FT_IRC_BOT"
# define POLL_TIMEOUT	2000

class Bot
{
	private:
		std::string	_botname;
		int	_socket;


	public:
	//	constructors  //
	
		Bot(int port, std::string password);
		Bot(const Bot &src);
		Bot &operator=(Bot const &src);
		~Bot(void);

	//	methods  //
	
		void serve(void);

	//	getters/setters  //
	
	private:
		Bot(void);
};

#endif
