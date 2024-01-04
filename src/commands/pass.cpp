/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pass.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/13 17:23:24 by shoddy        #+#    #+#                 */
/*   Updated: 2023/12/13 17:23:32 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

void	Command::pass(void)
{
	std::cout << ORANGE << "PASS called with: [" << _arguments[1] << "]\n" << RESET;
	std::cout << "Server PASS [" << _server.get_password() << "]\n";
	if (_arguments[1] == _server.get_password())
	{
		std::cout << PURPLE << "Authenticated user\n" << RESET;
		_caller.authenticate();
	}
	else
	{
		std::string wrong_pass;
		wrong_pass = "464 * :Password incorrect!";
		_caller.add_response(wrong_pass);
	}
	// else
	// {
	// 	std::string already_registered;
	// 	already_registered = SERVER_SIGNATURE;
	// 	already_registered += " 462 :That user is already registered";
	// 	_caller.add_response("::");
	// }
}