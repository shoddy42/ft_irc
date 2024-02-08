/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   user.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/13 16:49:45 by shoddy        #+#    #+#                 */
/*   Updated: 2023/12/13 17:19:39 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

void	Command::user(void)
{
	std::cout << ORANGE << "USER CALLED\n" << RESET; 
	std::string username = _arguments[1];
	std::string nickname = _arguments[2];

		// 	std::cout << "Responding to client" << std::endl;
		// 	std::string join_response(":localhost 001 jeff :Welcome to the IRC server, jeff!\n");
		// 	send(pollfds[sock].fd, join_response.c_str(), join_response.length(), 0);
	if (&_server.get_user(username) == &_server.get_user(-42))
	{
		//accept user because it doesnt already exist
		std::cout << PURPLE << "USER " << username << " registered\n" << RESET;
		std::string response = SERVER_SIGNATURE;
		//todo change message
		response += " 001 " + username + " :Welcome to the IRC server, " + username + "!\n";
		_caller.set_username(username);
		_caller.set_nickname(nickname);
		_caller.add_response(response);
		
	}
	else
	{
		//reprompt user, user already exists error.
		std::string response = "462 :Unauthorized command (already registered)";
		_caller.add_response(response);
	}
	
	
	// if (_caller.get_authenticated())
	
	
}