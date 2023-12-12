/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/10/18 11:39:34 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

	//  Variables   //

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
# include "../include/print.hpp"

# include <vector>
# include <map>
# include <unordered_map>

int error_exit(std::string error_msg);
int guard(int n, std::string error_msg);

// commands to fill the command_map
int quit(std::vector<std::string> command);
int join(std::vector<std::string> command);
int privmsg(std::vector<std::string> command);
int nick(std::vector<std::string> command);

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
		void	start(int port);

	// getters/setters
		void	add_user(int sock);
		User	&get_user(int sock);

	// networking
		void	accept_new_connection(void);
		void	socket_cleanup(int sock);

	// commands
		std::string	receive(int sock);
		void		respond(User &user);
		void		create_command(std::string buffer, User &caller);

		
	//	variables
		// poll[0] is the servers listening client. all others are the users. 
		std::vector<pollfd>	pollfds;
		std::list<User>		users;
		
	private:
		int	last_user_id = 0; //currently not used
		int	_listen_socket;


};


#endif