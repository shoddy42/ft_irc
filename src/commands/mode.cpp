#include "../../include/Command.hpp"

void	Command::mode_password(Channel &channel, bool is_plus, std::string password)
{
	if (is_plus == true)
	{
		channel.set_password(password);
		std::cout << "Password set to: " << password << std::endl;
	}
	else
	{
		channel.remove_password();
		std::cout << "Password removed" << std::endl;
	}
}

void	Command::mode_operator(Channel &channel, bool is_plus, User &user)
{
	if (is_plus == true)
	{
		if (channel.is_operator(user) == true)
			return; //usr alrd op
		else 
			channel.add_operator(user);
	}
	else 
	{
		if (channel.is_operator(user) == false)
			return; 
		else 
			channel.remove_operator(user);
	}
}

void	Command::mode_limit(Channel &channel, bool is_plus, int limit)
{
	if (is_plus == true)
		channel.set_user_limit(limit);
	else
		channel.set_user_limit(-1);
}

void	Command::mode_invite(Channel &channel, bool is_plus)
{
	if (channel.is_operator(_caller) == false)
		return;
	channel.set_invite_only(is_plus);
	if (is_plus == false)
		std::cout << ORANGE << "Channel is set to public" << std::endl;
	if (is_plus == true)
		std::cout << ORANGE << "Channel is set to invite only" << std::endl;
}

void	Command::mode(void)
{
	std::cout << ORANGE << "MODE called with: [" << _arguments[0] << " " << _arguments[1] << " " << _arguments[2] << " " << _arguments[3] << "]\n" << RESET;
	std::string flag = _arguments[2];
	Channel &channel = _server.get_channel(_arguments[1]);
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
	{

	}
	else if (flag[1] == 'o')
	{
		User &user = _server.get_user(_arguments[3]);
		mode_operator(channel, is_plus, user);
	}
	else if (flag[1] == 'k')
	{
		std::cout << "ATTEMPTING TO CHANGE PASSWORD TO: [" << _arguments[3] << "]\n";
		mode_password(channel, is_plus, _arguments[3]);
	}

}

