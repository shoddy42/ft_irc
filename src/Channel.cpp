/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Channel.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2023/09/05 18:43:58 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Channel::Channel(std::string channel_name, Server &server): _name(channel_name), _topic(DEFAULT_TOPIC), _server(server)
{
	_invite_only = false;
	_user_limit = -1;
	_topic_restricted = true;
	_password_required = false;
	_password = "";
}

Channel::Channel(const Channel &src): _server(src._server)
{
	if (this != &src)
		*this = src;
	_message_log = src._message_log;
	_user_list = src._user_list;
	_operator_list = src._operator_list;
	_name = src._name;
	_topic = src._topic;
	_password = src._password;
	_invite_only = src._invite_only;
	_user_limit = src._user_limit;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Channel::~Channel(void)
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Channel &Channel::operator=(Channel const &src)
{
	if (this == &src)
		return (*this);
	_message_log = src._message_log;
	_user_list = src._user_list;
	_operator_list = src._operator_list;
	_name = src._name;
	_topic = src._topic;
	_password = src._password;
	_invite_only = src._invite_only;
	_user_limit = src._user_limit;

	return (*this);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void	Channel::send_message(std::string &message, User &sender)
{
	std::cout << GREEN << "usr list size =  " << _user_list.size() << RESET << std::endl;
	if (is_user(sender) == false)
	{
		std::string reply = SERVER_SIGNATURE;
		reply += " 442 " + sender.get_nickname() + " " + get_name() + " :You are not in the channel " + get_name();
		sender.add_response(reply);
		return;
	}
	_message_log.push_back(message);
	for (std::list<User *>::iterator user = _user_list.begin(); user != _user_list.end() ; user++)
	{
		if (*user == &sender)
			continue;
		std::cout << GREEN << "added response to " << (*user)->get_nickname() << RESET << std::endl;
		(*user)->add_response(message);
	}
}

void	Channel::send_channel_info(User &user)
{
	//todo: maybe add 331 RPL_NOTOPIC
	//display the channels topic
	std::string topic = SERVER_SIGNATURE;
	topic += " 332 " + user.get_nickname() + " " + get_name() + " " + get_topic();
	user.add_response(topic);

	//display current users in the channel
	std::string name_reply = SERVER_SIGNATURE;
	name_reply += " 353 " + user.get_nickname() + " = " + get_name() + " :";
	for (std::list<User *>::iterator usr = _user_list.begin(); usr != _user_list.end(); usr++)
	{
		if (is_operator(**usr))
			name_reply += "@";
		name_reply += (*usr)->get_nickname() + " ";
	}
	user.add_response(name_reply);

	std::string end_of_names = SERVER_SIGNATURE;
	end_of_names += " 366 " + user.get_nickname() + " " + get_name() + " :End of /NAMES list.";
	user.add_response(end_of_names);

	//display the current modes of the channel
	std::string mode_reply = SERVER_SIGNATURE;
	mode_reply += " 324 " + user.get_nickname() + " " + get_name() + " +n";
	if (_topic_restricted)
		mode_reply += " +t";
	if (_password_required)
		mode_reply += " +k " + _password;
	if (_invite_only)
		mode_reply += " +i";
	user.add_response(mode_reply);

	//send end of WHO list for irssi sync
	std::string who_reply = SERVER_SIGNATURE;
	who_reply += " 315 " + user.get_nickname() + " " + get_name() + " :End of /WHO list";
	user.add_response(who_reply);

	//send end of channel ban list for irssi sync
	std::string ban_reply = SERVER_SIGNATURE;
	ban_reply += " 368 " + user.get_nickname() + " " + get_name() + " :End of channel ban list";
	user.add_response(ban_reply);
}

void	Channel::add_user(User &user)
{
	if (is_user(user))
		return;
	if (_user_limit > -1 && (int)_user_list.size() >= _user_limit)
		return;
	if (_invite_only == true)
		if (is_invited(user) == false)
			return;

	if (_user_list.size() == 0)
		_operator_list.push_back(&user);
	_user_list.push_back(&user);

	send_channel_info(user);

	//catch user up to all messages sent in the channel.
	for (std::vector<std::string>::iterator msg = _message_log.begin(); msg != _message_log.end(); msg++)
		user.add_response(*msg);
}

void	Channel::add_invited(User &user)
{
	_invited_list.push_back(&user);
}

void	Channel::add_operator(User &user)
{
	_operator_list.push_back(&user);
}

void	Channel::kick_user(User &user)
{
	for(std::list<User *>::iterator usr = _user_list.begin(); usr != _user_list.end(); usr++)
	{
		if (*usr == &user)
		{
			_user_list.erase(usr);
			remove_invited(user);
			//instant reply of error 442, to force irssi to close channel on leave.
			// std::string reply = SERVER_SIGNATURE;
			// reply += " 442 " + user.get_nickname() + " " + get_name() + " :You are not in the channel " + get_name();
			// user.add_response(reply);
			break;
		}
	}
}

void	Channel::remove_user(User &user, std::string reason)
{
	for(std::list<User *>::iterator usr = _user_list.begin(); usr != _user_list.end(); usr++)
	{
		if (*usr == &user)
		{
			std::string response = ":" + user.get_nickname() + "!" + user.get_username() + "@";
			response += HOSTNAME;
			response += " PART " + get_name() + " " + reason; //old
			// response += " PART :" + get_name();
			user.add_response(response);
			send_message(response, user);

			std::cout << "Removed user from: " << get_name() << std::endl;
			_user_list.erase(usr);
			remove_operator(user);
			//instant reply of error 442, to force irssi to close channel on leave.
			// std::string reply = SERVER_SIGNATURE;
			// reply += " 442 " + user.get_nickname() + " " + get_name() + " :You are not in the channel " + get_name();
			// user.add_response(reply);
			break;
		}
	}
}

void	Channel::remove_invited(User &user)
{
	for(std::list<User *>::iterator usr = _invited_list.begin(); usr != _invited_list.end(); usr++)
		if (*usr == &user)
		{
			_invited_list.erase(usr);
			break;
		}
}

void	Channel::remove_operator(User &user)
{
	for(std::list<User *>::iterator usr = _operator_list.begin(); usr != _operator_list.end(); usr++)
		if (*usr == &user)
		{
			_operator_list.erase(usr);
			break;
		}
}

void	Channel::remove_password(void)
{
	_password_required = false;
	_password = "";
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

const std::string	&Channel::get_name(void)
{
	return (_name);
}

const std::string 	&Channel::get_topic(void)
{
	return (_topic);
}

const std::string 	&Channel::get_password(void)
{
	return (_password);
}

void Channel::set_topic(std::string topic)
{
	_topic = topic;
}

void Channel::set_topic_restriction(bool deny_plebs)
{
	_topic_restricted = deny_plebs;
}

void Channel::set_password(std::string password)
{
	_password_required = true;
	_password = password;
}

void Channel::set_invite_only(bool is_private)
{
	_invite_only = is_private;
}

//a limit of -1 is no limit
void Channel::set_user_limit(int limit)
{
	_user_limit = limit;
}

bool Channel::has_password(void)
{
	return (_password_required);
}

bool Channel::is_topic_restricted(void)
{
	return (_topic_restricted);
}

bool Channel::is_operator(User &user)
{
	for(std::list<User *>::iterator usr = _operator_list.begin(); usr != _operator_list.end(); usr++)
		if (*usr == &user)
			return(true);
	return (false);
}

bool Channel::is_invited(User &user)
{
	if (_invite_only == false)
		return (true);
	for(std::list<User *>::iterator usr = _invited_list.begin(); usr != _invited_list.end(); usr++)
		if (*usr == &user)
			return(true);
	return (false);
}

bool Channel::is_user(User &user)
{
	for(std::list<User *>::iterator usr = _user_list.begin(); usr != _user_list.end(); usr++)
		if (*usr == &user)
			return(true);
	return (false);
}

/* ************************************************************************** */