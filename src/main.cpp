
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


#define BUFFER_SIZE	1024

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
	close(server.sock);

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

void accept_new_connection(void)
{
	int client_socket = 0;
	if (server.poll[0].revents & POLLIN)
	{
		std::cout << "New connection found. CONNECTING" << std::endl;
		//possible here to steal client information with sockaddr instead of nullptr.
		client_socket = guard(accept(server.sock, nullptr, nullptr), "Failed to accept socket. errno: ");
		int flags = guard(fcntl(client_socket, F_GETFL), "Fcntl failed to get flags. errno: ");
		guard(fcntl(client_socket, F_SETFL, flags | O_NONBLOCK), "Failed to set socket to non-blocking. errno: ");

		if (client_socket == -1)
			error_exit("Failed to accept TOS? errno: ");
		std::cout << "client socket created at: " << client_socket << std::endl;
		server.add_user(client_socket);
	}
}

int	main(int ac, char **av)
{
	//Signals to make ctrl+c to escape from "Accept" being stuck if no client connects.
	signal(SIGINT, sig_handler);

	if (ac == 3) //TODO: input parsing
		server.start(atoi(av[1]));
	else
		server.start(DEFAULT_PORT);


    while (escape == false)
	{
		if (poll(server.poll.data(), server.poll.size(), POLL_TIMEOUT) < 0)
			error_exit("poll failed. errno: ");

		accept_new_connection();

		if (server.poll.size() > 1)
		{
			if (server.poll[1].revents & POLLOUT)
			{
				std::string join_response(":server.name 001 userNickname :Welcome to the IRC server, userNickname!");
				send(server.poll[1].fd, join_response.c_str(), join_response.length(), 0);
			}
			if (server.poll[1].revents & POLLIN)
			{
				std::cout << "receiving: " << std::endl;
				char	buffer[BUFFER_SIZE];
				ssize_t	bytes_read = 0;

				if (bytes_read == 0)
					bytes_read = recv(server.poll[1].fd, buffer, BUFFER_SIZE, 0);
				if (bytes_read == -1) {
					// Handle error
					perror("read");
				} else if (bytes_read == 0) {
					// Connection closed by the client
					std::cout << "Client closed the connection." << std::endl;
				} else {
					std::string info(buffer);
					std::cout << info << std::endl;
					bzero(buffer, BUFFER_SIZE);
				}
			}
		}
	}
	return (0);
}