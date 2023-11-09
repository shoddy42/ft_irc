# include "../include/User.hpp"
# include "../include/Server.hpp"
# include "../include/Commands.hpp"


//check if
/* */
bool Commands::checkMsg(std::string &msg) 
{
    for (int i = 0; i < 0, i++)
    {
        if (buffer[i] == '\r' && (buffer[i + 1]) && buffer[i + 1] == '\n') 
            return true;
    }
    return false;
}

Commands::Commands()
{
    //inialize all commands
    ircCommands[0] = "QUIT";
    ircCommands[1] = "BAN";
    ircCommands[2] = "UNBAN";
    ircCommands[3] = "NICK";
    ircCommands[4] = "PASS";
    ircCommands[5] = "TOPIC";
    ircCommands[6] = "MODE";
    ircCommands[7] = "KILL";
    ircCommands[8] = "LIST";
    ircCommands[9] = "ME";
    ircCommands[10] = "INVITE";;
    ircCommands[11] = "OPER";
}

void Commands::parseCommand(std::string msg)
{

    size_t found;
    //checkMsg should be called here to check wether the msg is finished (ie. has a line break)

    /* this loop just checks for command within the string and if it finds it will then call the
    right command function*/
    for(int i = 0; i < ircCommands.length; i++)
    {
        found = msg.find(ircCommands[i]);
        if (found != std::string::npos) 
            //send to corrosponding command function
    }
    if (found == std::string::npos)
        std::cout << "no command found lol" << std::endl;
}