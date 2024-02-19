# include "../include/User.hpp"
# include "../include/Server.hpp"
# include "../include/Command.hpp"
# include "../include/print.hpp"

//check if
/* */
// bool Command::checkMsg(std::string &msg) 
// {
//     for (int i = 0; i < 0; i++)
//     {
//         if (buffer[i] == '\r' && (buffer[i + 1]) && buffer[i + 1] == '\n') 
//             return true;
//     }
//     return false;
// }

Command::~Command()
{

}

Command::Command(Server &server, User &caller): _server(server), _caller(caller)
{

}

std::map<std::string, Command::CommandFunction> Command::command_map = {
	{"NICK", &Command::nick},
	{"PING", &Command::ping},
	{"JOIN", &Command::join},
	{"PRIVMSG", &Command::privmsg},
	{"USER", &Command::user},
	{"KICK", &Command::kick},
	{"PASS", &Command::pass},
	{"TOPIC", &Command::topic},
	{"QUIT", &Command::quit},
	{"MODE", &Command::mode},
	{"INVITE", &Command::invite},
	{"PART", &Command::part},
};

void printVectorStrings(const std::vector<std::string> &strings)
{
	for (size_t i = 0; i < strings.size(); i++)
	{
		std::cout << strings[i].data() << " ";
	}
	std::cout << std::endl;
}

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
	if (command_map.find(key) != command_map.end())
	{
		if (key == "NICK")
			std::cout << "NICK HAS BEEN LOCATED!!!\n";
		CommandFunction function = command_map[key];
		(this->*function)();
	}
	else
		std::cerr << RED << "no command " << "\"" << _arguments[0] << "\" " << "found" << RESET << std::endl;
}