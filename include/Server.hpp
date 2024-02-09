/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2024/02/09 16:41:01 by shoddy        ########   odam.nl         */
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
# define BUFFER_SIZE  1024 //recommended to keep above 512.

# define INVALID_FD   -1

	//  Dependencies    //
# include "../include/User.hpp"
# include "../include/Command.hpp"
# include "../include/Channel.hpp"
# include "../include/print.hpp"

# include <netinet/in.h>
# include <sys/socket.h>
# include <strings.h>
# include <unistd.h>
# include <fcntl.h>
# include <poll.h>
# include <sstream>
# include <string>
# include <list>

# include <unordered_map>
# include <vector>
# include <map>

int			error_exit(std::string error_msg);
int			guard(int n, std::string error_msg);
int			parsePort(std::string portNumber);
std::string	parsePassword(std::string password);

class Channel;

class Server
{
	private:
		int			_listen_socket;
		std::string _password;

	public:
	//	variables
		std::vector<pollfd>	pollfds;  // pollfds[0] is the servers listening client. all others are the users. 
		std::list<User>		users;    // users[0] is a dummy user in case no user is found
		std::list<Channel>  channels; // channels[0] is a dummy channel in case no channel is found

	//	constructors
		Server(void);
		Server(const Server &src);
		~Server(void);
		Server &operator=(Server const &src);

	//	methods
		void		start(int port, std::string password);
		void		serve(void);
		std::string	receive(int sock);
		void		respond(User &user);
		void		create_command(std::string buffer, User &caller);

	// networking
		void		accept_new_connection(void);
		void		socket_cleanup(int sock);
		void		shutdown(void);
		void		delete_user(User &user);

	// getters/setters
		void		add_channel(std::string name);
		void		add_user(int sock);

		Channel		&get_channel(const std::string name);
		User		&get_user(std::string name);
		User		&get_user(int sock);
		std::string	&get_password(void);
};

#endif
