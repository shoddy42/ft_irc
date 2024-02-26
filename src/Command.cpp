# include "../include/User.hpp"
# include "../include/Server.hpp"
# include "../include/Command.hpp"
# include "../include/colours.hpp"

Command::~Command()
{
}

Command::Command(Server &server, User &caller): _server(server), _caller(caller)
{
}

std::map<std::string, Command::Function> Command::command_map = {
	{"PRIVMSG", &Command::privmsg},
	{"INVITE", &Command::invite},
	{"TOPIC", &Command::topic},
	{"NICK", &Command::nick},
	{"PING", &Command::ping},
	{"JOIN", &Command::join},
	{"USER", &Command::user},
	{"KICK", &Command::kick},
	{"PASS", &Command::pass},
	{"QUIT", &Command::quit},
	{"PART", &Command::part},
	{"MODE", &Command::mode},
	{"WHO", &Command::who},
};

void Command::add_argument(std::string argument)
{
	_arguments.push_back(argument);
}

void Command::execute(void)
{
	std::string key = _arguments[0].data();

	if (key == "CAP" || (key == "NICK" && _caller.get_authenticated() == false))
		return;
	if (_server.get_password() != "" && _caller.get_authenticated() == false && key != "PASS")
	{
		std::string wrong_pass = "464 * :Password incorrect!";
		_caller.add_response(wrong_pass);
		return;
	}
	if (command_map.find(key) == command_map.end())
	{
		std::cerr << RED << "no command " << "\"" << _arguments[0] << "\" " << "found" << RESET << std::endl;
		return;
	}
	Function function = command_map[key];
	(this->*function)();
}

/**
 * @brief Splits a given string on commas ','
 * 
 * @returns A vector of strings with the substrings excluding the commas ','
 */
std::vector<std::string> Command::split_argument(std::string input)
{
	std::vector<std::string>	args;
	size_t	pos;
	size_t	len;
	
	pos = input.find(',');
	args.push_back(input.substr(0, pos));
	if (pos == std::string::npos)
		return (args);
	len = pos + 1;
	while (len < input.length())
	{
		pos = input.find(',', len);
		if (pos == std::string::npos)
		{
			args.push_back(input.substr(len, std::string::npos));
			return (args);
		}
		args.push_back(input.substr(len, pos - len));
		len = pos + 1;
	}
	return (args);
}

bool Command::is_alnum(const std::string &str) {
    // for (char c : str)
	for (size_t i = 0; i < str.size(); i++)
        if (!std::isalnum(str[i]))
            return (false);
    return (true);
}