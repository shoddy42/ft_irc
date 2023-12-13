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
	std::string username = arguments[1];
	std::string nickname = arguments[2];

	if (&_server.get_user(username) != &_server.get_user(-42))
	{
		//accept user because it doesnt already exist

	}
	else
	{
		//reprompt user, user already exists error.
	}
	
	
	// if (_caller.get_authenticated())
	
	
}