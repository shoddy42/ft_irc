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

//todo: move off global server? its still needed for signal
Server server;

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
	// server.socket_cleanup(0);
	server.shutdown();
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
		//todo: socket cleanup, just like in error_exit
		exit(1);
    }
}

int	main(int ac, char **av)
{
	//Signals to make ctrl+c quit program as expected
	signal(SIGINT, sig_handler);

	if (ac >= 2) //TODO: input parsing
		server.start(atoi(av[1]));
	else
		server.start(DEFAULT_PORT);

    while (escape == false)
		server.serve();
	return (0);
}