/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pass.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/13 17:23:24 by shoddy        #+#    #+#                 */
/*   Updated: 2024/02/21 22:32:52 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

void	Command::pass(void)
{
	// std::cout << ORANGE << "PASS called with: [" << _arguments[1] << "]\n" << RESET;
	std::cout << "Server PASS [" << _server.get_password() << "]\n";
	if (_arguments[1] != _server.get_password())
	{
		std::string wrong_pass = "464 * :Password incorrect!";
		_caller.add_response(wrong_pass);
		return;
	}
	std::cout << PURPLE << "Authenticated user\n" << RESET;
	_caller.authenticate();
}