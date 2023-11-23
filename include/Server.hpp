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

# define MAX_CLIENTS 64
# define DEFAULT_PORT 6667
# define POLL_TIMEOUT 2000 //in ms

	//  Dependencies    //

# include <string>
# include <poll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <list>
# include <fcntl.h>

# include "../include/User.hpp"

# include <vector>
int error_exit(std::string error_msg);
int guard(int n, std::string error_msg);

class Server
{
	public:
	//	constructors
		Server(void);
		Server(const Server &src);
		~Server(void);
		Server &operator=(Server const &src);

	//	methods
		int     add_user(int sock);
		void	start(int port);

		
	//	variables
		int     sock;
		// poll[0] is the servers listening client. all others are the users. 
		std::vector<pollfd> poll; //daw: maybe rename this?
		std::list<User> users;
		
	private:
		int	current_users = 0;
		int	last_user_id = 0;

};


#endif