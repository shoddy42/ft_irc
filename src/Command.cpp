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
	//todo: checkMsg should be called here to check wether the msg is finished (ie. has a line break)
	std::string key = _arguments[0].data();

	if (key == "CAP" || (key == "NICK" && _caller.get_authenticated() == false))
		return;
	if (_server.get_password() != "" && _caller.get_authenticated() == false && key != "PASS")
		return;
	if (command_map.find(key) == command_map.end())
	{
		std::cerr << RED << "no command " << "\"" << _arguments[0] << "\" " << "found" << RESET << std::endl;
		return;
	}
	Function function = command_map[key];
	(this->*function)();
}