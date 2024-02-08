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

void	Command::mode(void)
{
	std::cout << ORANGE << "MODE called with: [" << _arguments[0] << _arguments[1] << _arguments[2] << "]\n" << RESET;
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
	}
	else if (flag[1] == 't')
	{
	}
	else if (flag[1] == 'o')
	{
		
	}
	else if (flag[1] == 'k')
	{
		std::cout << "ATTEMPTING TO CHANGE PASSWORD PLEASE\n";
		mode_password(channel, is_plus, _arguments[3]);
	}

}

