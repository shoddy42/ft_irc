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

// Command::Command():
// {
// 	//inialize all command
// 	// ircCommand[0] = "QUIT";
// 	// ircCommand[1] = "BAN";
// 	// ircCommand[2] = "UNBAN";
// 	// ircCommand[3] = "NICK";
// 	// ircCommand[4] = "PASS";
// 	// ircCommand[5] = "TOPIC";
// 	// ircCommand[6] = "MODE";
// 	// ircCommand[7] = "KILL";
// 	// ircCommand[8] = "LIST";
// 	// ircCommand[9] = "ME";
// 	// ircCommand[10] = "INVITE";;
// 	// ircCommand[11] = "OPER";
// }

std::map<std::string, Command::CommandFunction> Command::command_map = {
	{"NICK", &Command::nick},
	{"PING", &Command::ping},
	{"JOIN", &Command::join},
	{"PRIVMSG", &Command::privmsg},
	// {"QUIT", &quit},
};

void printVectorStrings(const std::vector<std::string> &strings)
{
	for (int i = 0; i < strings.size(); i++)
	{
		std::cout << strings[i].data() << " ";
	}
	std::cout << std::endl;
}

// int quit(std::vector<std::string> command)
// {
// 	return (0);
// }

// int join(std::vector<std::string> command)
// {
// 	return (0);

// }

// int privmsg(std::vector<std::string> command)
// {
// 	return (0);

// }

void Command::add_argument(std::string argument)
{
	arguments.push_back(argument);
}

void Command::execute(void)
{

	// size_t found;
	//checkMsg should be called here to check wether the msg is finished (ie. has a line break)

	std::string key = arguments[0].data();

	// std::cout << "Command " << arguments[0] << " found\n";
	if (command_map.find(key) != command_map.end())
	{
		CommandFunction function = command_map[key];
		if (function)
			(this->*function)();
	}
	else
		std::cerr << RED << "no command " << "\"" << arguments[0] << "\" " << "found" << RESET << std::endl;
	

}