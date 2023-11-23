# include "../include/User.hpp"
# include "../include/Server.hpp"
# include "../include/Commands.hpp"


//check if
/* */
// bool Commands::checkMsg(std::string &msg) 
// {
//     for (int i = 0; i < 0; i++)
//     {
//         if (buffer[i] == '\r' && (buffer[i + 1]) && buffer[i + 1] == '\n') 
//             return true;
//     }
//     return false;
// }

Commands::~Commands()
{

}

Commands::Commands()
{
    //inialize all commands
    // ircCommands[0] = "QUIT";
    // ircCommands[1] = "BAN";
    // ircCommands[2] = "UNBAN";
    // ircCommands[3] = "NICK";
    // ircCommands[4] = "PASS";
    // ircCommands[5] = "TOPIC";
    // ircCommands[6] = "MODE";
    // ircCommands[7] = "KILL";
    // ircCommands[8] = "LIST";
    // ircCommands[9] = "ME";
    // ircCommands[10] = "INVITE";;
    // ircCommands[11] = "OPER";
}


void printVectorStrings(const std::vector<std::string> &strings)
{
    for (int i = 0; i < strings.size(); i++)
    {
        std::cout << strings[i].data() << " ";
    }
    std::cout << std::endl;
}

int quit(std::vector<std::string> command)
{

}

int nick(std::vector<std::string> command)
{
    std::cout << "NICK COMMAND CALLED WITH: " << std::endl;

    printVectorStrings(command);
    return (0);
}

std::unordered_map<std::string, int(*)(std::vector<std::string> command)> Commands::commandMap = {
    {"QUIT", &quit},
    {"NICK", &nick}
};

void Commands::parseCommand(std::vector<std::string> command)
{

    // size_t found;
    //checkMsg should be called here to check wether the msg is finished (ie. has a line break)

    std::string key = command[0].data();

    if (commandMap.find(key) != commandMap.end())
        commandMap[key](command);
    else
        std::cerr << "no command found" << std::endl;
    

}