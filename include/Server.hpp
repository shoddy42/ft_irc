/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/12/13 16:52:30 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

	//  Variables   //
# define SERVER_SIGNATURE ":server_name"
# define HOSTNAME		  "serverhostname"

# define MAX_CLIENTS  512
# define DEFAULT_PORT 6667
# define POLL_TIMEOUT 2000 //in ms
# define BUFFER_SIZE  1024

# define INVALID_FD   -1

	//  Dependencies    //

# include <string>
# include <sstream>
# include <strings.h>
# include <poll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <list>
# include <fcntl.h>
# include <unistd.h>

# include "../include/User.hpp"
# include "../include/Command.hpp"
# include "../include/Channel.hpp"
# include "../include/print.hpp"

# include <vector>
# include <map>
# include <unordered_map>

int			error_exit(std::string error_msg);
int			guard(int n, std::string error_msg);
int			parsePort(std::string portNumber);
std::string	parsePassword(std::string password);

// commands to fill the command_map
int quit(std::vector<std::string> command);
int join(std::vector<std::string> command);
int privmsg(std::vector<std::string> command);
int nick(std::vector<std::string> command);

class Channel;

class Server
{
	public:
	//	constructors

		Server(void);
		Server(const Server &src);
		~Server(void);
		Server &operator=(Server const &src);

	//	methods

		void	serve(void);
		void	start(int port, std::string password);

	// getters/setters

		std::string	&get_password(void); //todo: make const?
		void		add_user(int sock);
		User		&get_user(int sock);
		User		&get_user(std::string name);
		Channel		&get_channel(const std::string name);
		void		add_channel();

	// networking

		void	accept_new_connection(void);
		void	socket_cleanup(int sock);
		void	shutdown(void);
		void	delete_user(User &user);


	// commands

		std::string	receive(int sock);
		void		respond(User &user);
		void		create_command(std::string buffer, User &caller);

		
	//	variables

		// pollfds[0] is the servers listening client. all others are the users. 
		std::vector<pollfd>	pollfds;
		// users[0] is a dummy user in case no user is found
		std::list<User>		users;
		// channels[0] is a dummy channel in case no channel is found
		std::list<Channel>  channels;
		
	private:
		int	_listen_socket;
		std::string _password;


};


#endif