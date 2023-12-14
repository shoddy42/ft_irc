//todo: delete the headers we dont need
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <csignal>
#include <chrono>
#include <poll.h>

#include "../include/Server.hpp"
#include "../include/Command.hpp"



bool escape = false;

//todo: figure out if there's another way to get server to signal?
Server *g_server;

//todo: decide whether guard is nicer than if statements
/**
 * @brief if n == -1, will exit the program, writing error_msg.
 * 		  use instead of small if blocks.
 * 
 * @param n 
 * @param error_msg 
 * @return int returns n
 */
int guard(int n, std::string error_msg)
{
	if (n == -1)
		error_exit(error_msg);
	return (n);
}

int	error_exit(std::string error_msg)
{
	std::cout << error_msg << errno << std::endl;
	//todo: sophisticated closing of all open sockets.
	g_server->shutdown();
	exit(EXIT_FAILURE);
}

/**
 * @brief Just makes ctrl+c quit the program when used with Signal()
 * 
 * @param signum automatically gets set by signals.
 * 
 */
void sig_handler(int signum)
{
    if (signum == SIGINT) {
		escape = true;
		g_server->shutdown();
		//todo: socket cleanup, just like in error_exit
		exit(EXIT_FAILURE);
    }
}

int	main(int ac, char **av)
{
	//Signals to make ctrl+c quit program as expected
	signal(SIGINT, sig_handler);
	Server server;
	g_server = &server;

	if (ac == 2)
		server.start(atoi(av[1]), "");
	else if (ac >= 3) //todo: input parsing
		server.start(atoi(av[1]), av[2]);
	else
		server.start(DEFAULT_PORT, "");
    while (escape == false)
		server.serve();
	return (0);
}