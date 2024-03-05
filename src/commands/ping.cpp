/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ping.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/05 15:05:20 by shoddy        #+#    #+#                 */
/*   Updated: 2024/03/05 15:44:59 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

void	Command::ping(void)
{
	std::string response = "PONG " + std::string(SERVER_SIGNATURE);
	_caller.add_response(response);
}