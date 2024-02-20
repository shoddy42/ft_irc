#include "../../include/Command.hpp"

void	Command::mode_password(Channel &channel, bool is_plus, std::string password)
{
	if (is_plus == true)
	{
		channel.set_password(password);
		std::cout << "Password set to: " << password << std::endl;
		std::string reply = SERVER_SIGNATURE;
		reply += " 324 " + _caller.get_nickname() + " " + channel.get_name() + " +k " + password + " : " + _caller.get_nickname();
		_caller.add_response(reply);

		return;
	}
	channel.remove_password();
	std::cout << "Password removed" << std::endl;
	std::string reply = SERVER_SIGNATURE;
	reply += " 324 " + _caller.get_nickname() + " " + channel.get_name() + " -k " + password + " : " + _caller.get_nickname();
	_caller.add_response(reply);
}

void	Command::mode_operator(Channel &channel, bool is_plus, User &user)
{
	if (is_plus == false)
	{
		std::string reply = SERVER_SIGNATURE;
		reply += " 324 " + user.get_nickname() + " " + channel.get_name() + " -o : " + user.get_nickname();
		user.add_response(reply);

		channel.remove_operator(user);
		return;
	}
	if (channel.is_operator(user) == true)
		return; //usr alrd op
	std::string reply = SERVER_SIGNATURE;
	reply += " 324 " + user.get_nickname() + " " + channel.get_name() + " +o : " + user.get_nickname();
	user.add_response(reply);

	channel.add_operator(user);
}

void	Command::mode_limit(Channel &channel, bool is_plus, int limit)
{
	if (is_plus == true)
	{
		channel.set_user_limit(limit);
		std::string reply = SERVER_SIGNATURE;
		reply += " 324 " + _caller.get_nickname() + " " + channel.get_name() + " +l : " + _caller.get_nickname();
		_caller.add_response(reply);
	}
	else
	{
		channel.set_user_limit(-1);
		std::string reply = SERVER_SIGNATURE;
		reply += " 324 " + _caller.get_nickname() + " " + channel.get_name() + " -l : " + _caller.get_nickname();
		_caller.add_response(reply);
	}
}

void	Command::mode_topic(Channel &channel, bool is_plus)
{
	if (is_plus == true)
	{
		channel.set_topic_restriction(is_plus);
		std::string reply = SERVER_SIGNATURE;
		reply += " 324 " + _caller.get_nickname() + " " + channel.get_name() + " +t : " + _caller.get_nickname();
		_caller.add_response(reply);
	}
	else
	{
		channel.set_topic_restriction(is_plus);
		std::string reply = SERVER_SIGNATURE;
		reply += " 324 " + _caller.get_nickname() + " " + channel.get_name() + " -t : " + _caller.get_nickname();
		_caller.add_response(reply);
	}
}

void	Command::mode_invite(Channel &channel, bool is_plus)
{
	channel.set_invite_only(is_plus);
	if (is_plus == false)
	{
		std::cout << ORANGE << "Channel is set to public" << std::endl;
		std::string reply = SERVER_SIGNATURE;
		reply += " 324 " + _caller.get_nickname() + " " + channel.get_name() + " -i : " + _caller.get_nickname();
		_caller.add_response(reply);
	}
	if (is_plus == true)
	{
		std::cout << ORANGE << "Channel is set to invite only" << std::endl;
		std::string reply = SERVER_SIGNATURE;
		reply += " 324 " + _caller.get_nickname() + " " + channel.get_name() + " +i : " + _caller.get_nickname();
		_caller.add_response(reply);
	}
}

void	Command::mode(void)
{
	std::cout << ORANGE << "MODE called\n" << RESET;
	std::string flag = "illegal flag";
	if (_arguments.size() > 2)
		flag = _arguments[2];
	Channel &channel = _server.get_channel(_arguments[1]);
	if (channel.is_operator(_caller) == false)
		return;
	if (flag.empty())
		return; //or error
	if(flag[0] != '-' && flag[0] != '+')
		return;
	bool is_plus = false;
	if (flag[0] == '+')
		is_plus = true;	

	std::cout << "TEST: " << flag << std::endl;
	if (flag[1] == 'i')
	{
		mode_invite(channel, is_plus);
	}
	else if (flag[1] == 'l')
	{
		int limit = 0;
		try
		{
			limit = std::stoi(_arguments[3]); 
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
			return;
		}
		mode_limit(channel, is_plus, limit);
	}
	else if (flag[1] == 't')
		mode_topic(channel, is_plus);
	else if (flag[1] == 'o')
	{
		User &user = _server.get_user(_arguments[3]);
		mode_operator(channel, is_plus, user);
	}
	else if (flag[1] == 'k')
		mode_password(channel, is_plus, _arguments[3]);
}
