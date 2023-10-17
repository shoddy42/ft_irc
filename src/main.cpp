
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

int	error_exit(std::string msg)
{
	std::cout << msg << errno << std::endl;
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
		client_socket = accept(server.sock, nullptr, nullptr);
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
			if (server.poll[1].revents & POLLOUT) //currently if statements seem broke
			{
				send(server.poll[1].fd, "joebiden", 8, 0);
			}
			if (server.poll[1].revents & POLLIN)
			{
				std::cout << "receiving" << std::endl;
				char	buffer[BUFFER_SIZE];
				ssize_t	bytes_read = 0;

				if (bytes_read == 0)
					bytes_read = recv(server.sock, buffer, BUFFER_SIZE, 0);
				std::string info(buffer);
				std::cout << info;
				if (bytes_read == -1) {
					// Handle error
					perror("read");
				} else if (bytes_read == 0) {
					// Connection closed by the client
					std::cout << "Client closed the connection." << std::endl;
					exit (0); //remove this later, just needed to prevent the giga spam rn
				} else {
					// Process and handle the received data
					buffer[bytes_read] = '\0'; // null terminate the received data
					std::cout << "Received data from client: " << buffer << std::endl;
				}
			}
		}
	}
	return (0);
}