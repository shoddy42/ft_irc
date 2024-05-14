/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   pass.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/12/13 17:23:24 by shoddy        #+#    #+#                 */
/*   Updated: 2024/05/14 18:50:51 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

void	Command::pass(void)
{
	std::string password = _arguments[1];
	if (password.length() > 1 && password[0] == ':')
		password = password.erase(0, 1);

	std::cout << "Server PASS [" << _server.get_password() << "]\n";
	if (password != _server.get_password())
	{
		std::string wrong_pass = "464 * :Password incorrect!";
		_caller.add_response(wrong_pass);
		return;
	}
	std::cout << PURPLE << "Authenticated user\n" << RESET;
	_caller.authenticate();
}