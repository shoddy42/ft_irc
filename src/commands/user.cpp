/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   user.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/13 16:49:45 by shoddy        #+#    #+#                 */
/*   Updated: 2024/03/05 15:39:28 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

void	Command::user(void)
{
	std::string username = limit_name_length(_arguments[1]);
	std::string nickname = limit_name_length(_arguments[2]);
	std::string hostname = limit_name_length(_arguments[3]);
	std::string realname = limit_name_length(_arguments[4]);

	if (_server.get_user(username).get_username() != NULL_USER) //reprompt user, user already exists error.
	{
		std::string response = "462 :Unauthorized command (already registered)";
		_caller.add_response(response);
		return;
	}
	//accept user because it doesnt already exist
	std::cout << PURPLE << "USER " << username << " registered" << std::endl << RESET;
	std::string response = std::string(SERVER_SIGNATURE) + " 001 " + username + " " + std::string(RPL_WELCOME);
	_caller.set_username(username);
	_caller.set_nickname(nickname);
	_caller.set_hostname(hostname);
	_caller.set_realname(realname);
	_caller.add_response(response);
	
	std::string creation_date = std::string(SERVER_SIGNATURE) + " 003 " + username + " :This server was created " + _server.get_creation_time();
	_caller.add_response(creation_date);
	if (_server.get_password() == "")
		_caller.authenticate();
	_caller.sign_up();
	std::string reply = usermask(_caller) + " NICK " + nickname;
	_caller.add_response(reply);
}