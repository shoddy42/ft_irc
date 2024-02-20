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
Server *g_server;

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
	std::cerr << error_msg << errno << std::endl;
	g_server->shutdown();
	escape = true;
	return (1);
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
    }
}

int	main(int ac, char **av)
{
	//Signals to make ctrl+c quit program as expected
	Server server;
	g_server = &server;
	signal(SIGINT, sig_handler);

	// if (ac == 3) // todo: comment in
	// 	server.start(guard(parsePort(av[1]), "Incorrect Input, errno: "), av[2]);
	// else
	// {
	// 	std::cout << RED << "Error: Please Provide The Correct Number Of Arguments.\n" << GREEN << "Usage: " << av[0] << " <Port> <Password>" << RESET << std::endl;
	// 	return (1);
	// }

	if (ac == 2)
		server.start(guard(parsePort(av[1]), "Incorrect Input, errno: "), "");
	else if (ac >= 3)
		server.start(guard(parsePort(av[1]), "Incorrect Input, errno: "), parsePassword(av[2]));
	else
		server.start(DEFAULT_PORT, ""); // todo: comment out
	
    while (escape == false)
		server.serve();
	// Cleanup?
	return (0);
}


/*
	SEGFAULTS!:


	OTHER ISSUES ISSUES:
	Fixes:
-kick doesn't display any messages on the client side
-mode doesn't work with +t/-t for TOPIC


	Potential Fixes:
-the extra screen close after leaving, since it trips up /wc


*/
