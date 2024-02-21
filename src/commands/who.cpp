/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   who.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/02/21 10:07:06 by shoddy        #+#    #+#                 */
/*   Updated: 2024/02/21 17:00:32 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

// #include "../../include/User.hpp"
// #include "../../include/Channel.hpp"
#include "../../include/Command.hpp"



void	Command::who(void)
{
	if (_arguments.size() < 2)
		return;
	Channel &channel = _server.get_channel(_arguments[1]);
	if (channel.get_name() == NULL_CHANNEL_NAME)
		return;

	channel.who(_caller);
}