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
        // room for port limitations/rules
        if (port < 1)
            throw (std::invalid_argument("The Port Must Be A Non-Zero Number"));
    }
    catch (std::exception &e)
    {
        // std::cout << e.what() << std::endl;
        std::cout << "The Port Must Be A Non-Zero Number" << std::endl;
        return (-1);
    }
    return (port);
}

std::string parsePassword(std::string password)
{
    // room for (arbitrary) rules and conditions

    return (password);
    // error_exit("This Password Is Not Allowed");
}
