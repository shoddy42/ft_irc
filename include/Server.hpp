/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2024/03/05 17:21:03 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

	//  Variables   //
	
# define SERVER_SIGNATURE ":ft.irc.com"
# define RPL_WELCOME "Welcome to the FT_IRC!"

# define MAX_CLIENTS  512 //maximum clients the server will accept.
# define MAX_LENGTH   512 //maximum length for user/nick/real/channel names. Sadly bugged with irssi, keep at 512.
# define DEFAULT_PORT 6667 //the default TCP/IP port to use for the server
# define POLL_TIMEOUT 2000 //in ms

	// Variables you shouldn't change //

# define BUFFER_SIZE  1024 //Amount of data read by receive. recommended to keep above 512.
# define INVALID_FD   -1 //the default FD set to invalid.
# define NULL_CHANNEL_NAME "null channel" //the empty channel created by the server to ensure safety. 

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
void				error_exit(std::string error_msg);
int					guard(int n, std::string error_msg);
int					parsePort(std::string portNumber);
bool				parsePassword(const std::string &password);
std::string			limit_name_length(std::string name);

class Server
{
	private:
		int					_listen_socket;
		std::string 		_password;
		std::string			_creation_time;
		std::vector<pollfd>	pollfds;  // pollfds[0] is the servers listening client. all others are the users. 
		std::list<Channel>  channels; // channels[0] is a null channel in case no channel is found
		std::list<User>		users;    // users[0] is a null user in case no user is found
		std::vector<std::string> _unfinished_packets; // stores packets until newline is found

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
		void		respond_(User &user);

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
