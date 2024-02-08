#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "../include/Command.hpp"
#include "../include/print.hpp"


int parsePort(std::string portNumber)
{
    int port;

    port = -1;
    try
    {
        port = std::stoi(portNumber);
        if (port < 1024)
            throw (std::invalid_argument("The Port Must Be A Positive Number Above '1023'"));
    }
    catch (std::exception &e)
    {
        // std::cout << e.what() << std::endl;
        std::cout << "The Port Must Be A Positive Number Above '1023'" << std::endl;
        return (-1);
    }
    return (port);
}

std::string parsePassword(std::string password)
{
    //123
    return (password);
}
