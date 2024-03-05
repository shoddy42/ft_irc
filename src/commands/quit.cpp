/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   quit.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/05 15:05:09 by shoddy        #+#    #+#                 */
/*   Updated: 2024/03/05 15:05:09 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

void	Command::quit(void)
{
	_server.remove_user(_caller);
}