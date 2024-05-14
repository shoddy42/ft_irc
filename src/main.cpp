/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/05 15:07:00 by shoddy        #+#    #+#                 */
/*   Updated: 2024/05/14 17:08:00 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include "../include/Command.hpp"

#include <csignal>

//global bool required for signal to properly quit the program on CTRL+C
bool escape = false;
//global server required for cleanup of memory on server exit with CTRL+C or on error.
Server *g_server;

/**
 * @brief if n == -1, will exit the program, writing error_msg.
 * 		  use instead of small if blocks.
 * 
 * @return @b int n
 */
int guard(int n, std::string error_msg)
{
	if (n == -1)
		error_exit(error_msg);
	return (n);
}

void	error_exit(std::string error_msg)
{
	std::cerr << error_msg << errno << std::endl;
	escape = true;
	g_server->shutdown();
	exit(EXIT_SUCCESS);
}

/**
 * @brief Just makes ctrl+c quit the program when used with Signal()
 * 
 * @param signum automatically gets set by signals.
 */
void sig_handler(int signum)
{
	if (signum == SIGINT)
		escape = true;
}

/**
 * @brief Creates the full usermask. "nick!username\@hostname".
 * 		  No space at the end.
 */
const std::string usermask(User &user)
{
	std::string mask = ":" + user.get_nickname() + "!" + user.get_username() + "@" + user.get_hostname();
	return (mask);
}

int	main(int ac, char **av)
{
	// if (ac != 3)
	// {
	// 	std::cout << RED << "Error: Please Provide The Correct Number Of Arguments.\n" << GREEN << "Usage: " << av[0] << " <Port> <Password>" << RESET << std::endl;
	// 	return (1);
	// }
	int port = parsePort(av[1]);
	std::string password = av[2]; 
	if (port == -1)
		return (1);
	if (parsePassword(password) == false)
		return (1);
		
	Server server;
	g_server = &server;
	signal(SIGINT, sig_handler); //Signals to make ctrl+c quit program as expected

	// server.start(port, password);

	if (ac == 2)
		server.start(guard(parsePort(av[1]), "Incorrect Input, errno: "), "");
	else if (ac >= 3)
		server.start(guard(parsePort(av[1]), "Incorrect Input, errno: "), password);
	else
		server.start(DEFAULT_PORT, "");
	
	while (escape == false)
		server.serve();
	server.shutdown();
	return (0);
}
