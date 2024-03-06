/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   mode.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     add_userv*/
/*                                                   +#+                      */
/*   Created: 2024/03/05 15:05:52 by shoddy        #+#    #+#                 */
/*   Updated: 2024/03/06 03:37:49 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/Command.hpp"

std::string	Command::mode_password(Channel &channel, bool is_plus, std::string password)
{
	if (is_plus == true)
	{
		if (channel.get_password() == password)
			return ("");
		channel.set_password(password);
		std::cout << "Password set to: " << password << std::endl;
		return (usermask(_caller) + " MODE " + channel.get_name() + " +k");
	}
	if (channel.has_password() == false)
		return ("");
	channel.remove_password();
	std::cout << "Password removed" << std::endl;
	return (usermask(_caller) + " MODE " + channel.get_name() + " -k");	
}

std::string	Command::mode_limit(Channel &channel, bool is_plus, std::string str_limit)
{
	int limit = -1;
	try
	{
		limit = std::stoi(str_limit); 
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return ("");
	}
	if (is_plus == true)
	{
		if (channel.get_user_limit() == limit)
			return ("");
		channel.set_user_limit(limit);
		return (usermask(_caller) + " MODE " + channel.get_name() + " +l");
	}
	if (channel.get_user_limit() == -1)
		return ("");
	channel.set_user_limit(-1);
	return (usermask(_caller) + " MODE " + channel.get_name() + " -l");	
}

std::string	Command::mode_operator(Channel &channel, bool is_plus, User &user)
{
	if (is_plus == false)
	{
		if (channel.is_operator(user) == false)
			return ("");
		channel.remove_operator(user);
		return (usermask(_caller) + " MODE " + channel.get_name() + " -o :" + user.get_nickname());
	}
	if (channel.is_operator(user) == true)
		return ("");
	channel.add_operator(user);
	return (usermask(_caller) + " MODE " + channel.get_name() + " +o :" + user.get_nickname());
}


std::string	Command::mode_topic(Channel &channel, bool is_plus)
{
	if (is_plus == true)
	{
		if (channel.is_topic_restricted() == true)
			return ("");
		channel.set_topic_restriction(is_plus);
		return (usermask(_caller) + " MODE " + channel.get_name() + " +t");
	}
	if (channel.is_topic_restricted() == false)
		return ("");
	channel.set_topic_restriction(is_plus);
	return (usermask(_caller) + " MODE " + channel.get_name() + " -t");
}

std::string	Command::mode_invite(Channel &channel, bool is_plus)
{
	channel.set_invite_only(is_plus);
	if (is_plus == true)
	{
		if (channel.is_invite_only() == true)
			return ("");
		return (usermask(_caller) + " MODE " + channel.get_name() + " +i");
	}
	if (channel.is_invite_only() == false)
		return ("");
	return (usermask(_caller) + " MODE " + channel.get_name() + " -i");
}

void	Command::mode_user(User &user, std::string flag)
{
	if (flag == "+i")
	{
		user.add_response(std::string(SERVER_SIGNATURE) + " MODE " + user.get_nickname() + " " + flag);
		return;
	}
	user.add_response(std::string(SERVER_SIGNATURE) + " MODE " + user.get_nickname() + " " + flag);
}

void	Command::mode_channel(Channel &channel)
{
	if (_arguments.size() == 3 && _arguments[2] == "b")
	{
		std::string ban_reply = SERVER_SIGNATURE;
		ban_reply += " 368 " + _caller.get_nickname() + " " + channel.get_name() + " :End of channel ban list";
		_caller.add_response(ban_reply);
		return;
	}
	channel.mode(_caller);
}

void	Command::mode(void)
{
	std::string flag = "";
	if (_arguments.size() > 2)
		flag = _arguments[2];
	Channel &channel = _server.get_channel(_arguments[1]);
	if ((flag.empty() || flag == "b") && channel.get_name() != NULL_CHANNEL_NAME)
		mode_channel(channel);
	if (channel.is_operator(_caller) == false)
		return;

	if(flag[0] != '-' && flag[0] != '+')
		return;
	bool is_plus = false;
	if (flag[0] == '+')
		is_plus = true;	
	if (channel.get_name() == NULL_CHANNEL_NAME)
	{
		User &user = _server.get_user(_arguments[2]);
		if (user.get_socket() == -42)	
			return;
		mode_user(user, flag);
		return;
	}

	std::string reply;
	if (flag[1] == 'l')
		reply = mode_limit(channel, is_plus, _arguments[3]);
	else if (flag[1] == 'i')
		reply = mode_invite(channel, is_plus);
	else if (flag[1] == 't')
		reply = mode_topic(channel, is_plus);
	else if (flag[1] == 'o')
		reply = mode_operator(channel, is_plus, _server.get_user(_arguments[3]));
	else if (flag[1] == 'k')
		reply = mode_password(channel, is_plus, _arguments[3]);
	channel.send_notice(reply);
}
