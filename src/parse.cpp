#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "../include/Command.hpp"
#include "../include/colours.hpp"

/**
 * @brief Parses portNumber, Throwing An Exception For Non-Numerical Input and ports Below 1024
 * 
 * @param portNumber 
 * @return int 
 */
int parsePort(std::string portNumber)
{
    int port;

    port = -1;
    try
    {
        port = std::stoi(portNumber);
        if (port < 1024 || port > 65535)
            throw (std::invalid_argument("The Port Must Be Between '1023' And '65535'"));
    }
    catch (std::exception &e)
    {
        // std::cout << e.what() << std::endl;
        std::cout << "The Port Must Be Between '1023' And '65535'" << std::endl;
        return (-1);
    }
    return (port);
}

// todo: actual parsing, alphanum only, < 500
/**
 * @brief Parses password, If Need Be.
 * 
 * @param password 
 * @return std::string 
 */
std::string parsePassword(std::string password)
{
    //123
    return (password);
}
