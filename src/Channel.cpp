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

Channel::Channel(std::string channel_name, Server &server): _name(limit_name_length(channel_name)), _topic(""),  _server(server)
{
	_invite_only = false;
	_user_limit = -1;
	_topic_restricted = DEFAULT_RESTRICT;
	_password_required = false;
	_password = "";

    // get the current time
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    // convert the time to a duration since epoch
    std::chrono::system_clock::duration duration = now.time_since_epoch();
    // convert the duration to seconds
    std::chrono::seconds seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
	_creation_time = seconds.count();
}

Channel::Channel(const Channel &src): _server(src._server)
{
	if (this != &src)
		*this = src;
	_message_log = src._message_log;
	_operator_list = src._operator_list;
	_invited_list = src._invited_list;
	_user_list = src._user_list;
	_name = src._name;
	_topic = src._topic;

	_password = src._password;
	_user_limit = src._user_limit;
	_password_required = src._password_required;
	_topic_restricted = src._topic_restricted;
	_invite_only = src._invite_only;
	_creation_time = src._creation_time;
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

/**
 * @brief Sends a message to all users in the channel, except the sender.
 * 		  Adds the sent message to the channels message_log
 */
void	Channel::send_message(std::string &message, User &sender)
{
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
		// std::cout << GREEN << "added response to " << (*user)->get_nickname() << RESET << std::endl;
		(*user)->add_response(message);
	}
}

/**
 * @brief Sends a message to ALL users in the channel. This message does NOT get logged.
 */
void	Channel::send_notice(std::string &message)
{
	for (std::list<User *>::iterator user = _user_list.begin(); user != _user_list.end() ; user++)
		(*user)->add_response(message);
}

void	Channel::send_channel_info(User &user)
{
	//display the channels topic
	if (!_topic.empty())
	{
		std::string topic = SERVER_SIGNATURE;
		topic += " 332 " + user.get_nickname() + " " + get_name() + " " + _topic;
		user.add_response(topic);
	}
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
}

void	Channel::who(User &caller)
{
	for (std::list<User *>::iterator user = _user_list.begin(); user != _user_list.end() ; user++)
	{
		std::string reply = std::string(SERVER_SIGNATURE) + " 352 " + caller.get_nickname() + " ";
		reply += get_name() + " " + caller.get_username() + " " + caller.get_hostname() + " localhost " + caller.get_nickname() + " H";
		if (is_operator(caller))
			reply += "@";
		reply += " :0 " + caller.get_realname();
		caller.add_response(reply);
	}
	
	std::string who_reply = SERVER_SIGNATURE;
	who_reply += " 315 " + caller.get_nickname() + " " + get_name() + " :End of /WHO list";
	caller.add_response(who_reply);
}

void	Channel::mode(User &caller)
{
	std::string mode_reply = std::string(SERVER_SIGNATURE) + " 324 " + caller.get_nickname() + " " + get_name() + " +n";
	if (_topic_restricted)
		mode_reply += "t";
	if (_password_required)
		mode_reply += "k";
	if (_invite_only)
		mode_reply += "i";
	caller.add_response(mode_reply);

	std::string time_reply = std::string(SERVER_SIGNATURE) + " 329 " + caller.get_username() + " " + get_name() + " " + std::to_string(_creation_time);
	caller.add_response(time_reply);
}

/**
 * @brief Adds a user to a channel. Also sends all needed information about the channel.
 */
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
		add_operator(user);
	_user_list.push_back(&user);

	send_channel_info(user);

	//catch user up to all messages sent in the channel.
	for (std::vector<std::string>::iterator msg = _message_log.begin(); msg != _message_log.end(); msg++)
		user.add_response(*msg);
}

void	Channel::add_invited(User &user)
{
	if (is_invited(user) == false)
		_invited_list.push_back(&user);
}

void	Channel::add_operator(User &user)
{
	if (is_operator(user) == false)
		_operator_list.push_back(&user);
}

/**
 * @brief Removes a user from a channel, also takes away operator.
 * 		  Sends a notice to the entire channel.
 * 		  Will delete the channel if the last user is removed.
 * 
 * @return @b true if the channel was deleted. @b false if the channel still exists.
 */
bool	Channel::remove_user(User &user, std::string reason)
{
	std::cout << PURPLE << "Remove user called\n" << RESET;
	for(std::list<User *>::iterator usr = _user_list.begin(); usr != _user_list.end(); usr++)
	{
		if (*usr == &user)
		{
			std::cout << "Removed user from: " << get_name() << std::endl;
			std::string notice = usermask(user) + " PART " + get_name() + reason;
			send_notice(notice);

			_user_list.erase(usr);
			remove_operator(user);
			if (_user_list.size() < 1 && get_name() != NULL_CHANNEL_NAME)
			{
				_server.remove_channel(*this);
				return (true);
			}
			break;
		}
	}
	return (false);
}

void	Channel::remove_invited(User &user)
{
	for(std::list<User *>::iterator usr = _invited_list.begin(); usr != _invited_list.end(); usr++)
	{
		if (*usr == &user)
		{
			_invited_list.erase(usr);
			break;
		}
	}
}

void	Channel::remove_operator(User &user)
{
	for(std::list<User *>::iterator usr = _operator_list.begin(); usr != _operator_list.end(); usr++)
	{
		if (*usr == &user)
		{
			_operator_list.erase(usr);
			break;
		}
	}
}

void	Channel::remove_password(void)
{
	_password_required = false;
	_password = "";
}

void	Channel::kick_user(User &user)
{
	for(std::list<User *>::iterator usr = _user_list.begin(); usr != _user_list.end(); usr++)
	{
		if (*usr == &user)
		{
			_user_list.erase(usr);
			remove_invited(user);
			remove_operator(user);
			break;
		}
	}
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