/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Server.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/10/17 13:29:37 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef Server_HPP
# define Server_HPP
# include <string>
# include <poll.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <vector>

# include "../include/User.hpp"

class Server
{
	public:
		Server(void);
		Server(const Server &src);
		~Server(void);
		Server &operator=(Server const &src);

        //methods
        int     add_user(void);
        
        //variables
        int     listen_socket;
        pollfd  poll_data;
		
	private:
        std::vector<User> users;

};


#endif