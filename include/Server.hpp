/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2024/02/22 00:05:26 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

	//  Variables   //
	
# define SERVER_SIGNATURE ":ft.irc.com"
# define RPL_WELCOME "Welcome to the FT_IRC!"


# define MAX_CLIENTS  512
# define DEFAULT_PORT 6667
# define POLL_TIMEOUT 2000 //in ms
# define BUFFER_SIZE  1024 //recommended to keep above 512.

# define INVALID_FD   -1
# define NULL_CHANNEL_NAME "null channel"

	//  Dependencies    //
	
# include "../include/User.hpp"
# include "../include/Command.hpp"
# include "../include/Channel.hpp"
# include "../include/colours.hpp"

# include <netinet/in.h>
# include <sys/socket.h>
# include <strings.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>

# include <sstream>
# include <string>
# include <chrono>
# include <ctime>
# include <list>

# include <unordered_map>
# include <vector>
# include <map>

class Channel;

const std::string	usermask(User &user);
int					error_exit(std::string error_msg);
int					guard(int n, std::string error_msg);
int					parsePort(std::string portNumber);
std::string			parsePassword(std::string password);

class Server
{
	private:
		int					_listen_socket;
		std::string 		_password;
		std::string			_creation_time;
		std::vector<pollfd>	pollfds;  // pollfds[0] is the servers listening client. all others are the users. 
		std::list<Channel>  channels; // channels[0] is a null channel in case no channel is found
		std::list<User>		users;    // users[0] is a null user in case no user is found

	public:
	//	constructors  //
	
		Server(void);
		Server(const Server &src);
		Server &operator=(Server const &src);
		~Server(void);

	//	methods  //
	
		void		start(int port, std::string password);
		void		serve(void);
		std::string	receive(int sock);
		void		respond(User &user);

		void		do_command(std::string buffer, User &caller);
		void		add_channel(std::string name, User &creator);
		void		add_user(int sock);
	
		void		remove_channel(Channel &channel);
		void		remove_user(User &user);

	//	networking  //

		void		accept_new_connection(void);
		void		socket_cleanup(int sock);
		void		shutdown(void);

	//	getters/setters  //
	
		Channel		&get_channel(const std::string name);
		User		&get_user(std::string name);
		User		&get_user(int sock);
		std::string	&get_password(void);
		std::string	&get_creation_time(void);
};

#endif
