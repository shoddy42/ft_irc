
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

/**
 * @brief Checks if our listening socket has been contacted. Then creates a user.
 * 
 */
void accept_new_connection(void)
{
	if (server.poll[0].revents & POLLIN)
	{
		std::cout << "New connection found. CONNECTING" << std::endl;
		//possible here to steal client information with sockaddr instead of nullptr.
		int client_socket = guard(accept(server.sock, nullptr, nullptr), "Failed to accept socket. errno: ");
		int flags = guard(fcntl(client_socket, F_GETFL), "Fcntl failed to get flags. errno: ");
		guard(fcntl(client_socket, F_SETFL, flags | O_NONBLOCK), "Failed to set socket to non-blocking. errno: ");
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
		//Poll every socket we have.
		// if (poll(server.poll.data(), server.poll.size(), POLL_TIMEOUT) < 0)
		// 	error_exit("poll failed. errno: ");

		//Check all open sockets, to see if any have written to us.
		
		guard(poll(server.poll.data(), server.poll.size(), POLL_TIMEOUT), "poll failed. errno: ");

		accept_new_connection();
		for (size_t i = 1; i < server.poll.size(); i++)
		{
			if (server.poll[i].revents == 0) //client hasnt done anything
				continue;
			if (server.poll[i].revents & POLLHUP) //client disconnected
			{
				std::cout << "Socket " << i << " hung up." << std::endl;
				exit(0);
				// continue;
				//socket cleanup
			}
			// All commented out because we should simply reply after we've received the message.
			// if (server.poll[i].revents & POLLOUT)
			// {
			// 	// if (response_counter == 0)
			// 	// {
			// 	// 	std::string join_response(":server.name 001 userNickname :Welcome to the IRC server, userNickname!");
			// 	// 	send(server.poll[i].fd, join_response.c_str(), join_response.length(), 0);
			// 	// 	response_counter++;
			// 	// }
			// }
			if (server.poll[i].revents & POLLIN) //client sent server a message
			{
				std::cout << "receiving from socket [" << i << "]: "<< std::endl;
				char	buffer[BUFFER_SIZE];
				ssize_t	bytes_read;

				bzero(buffer, BUFFER_SIZE);
				bytes_read = recv(server.poll[i].fd, buffer, BUFFER_SIZE, 0);
				if (bytes_read == -1) {		  // Recv failed
					perror("recv");
				} else if (bytes_read == 0) { // Connection closed by the client //might not be needed, POLLHUP should already catch
					std::cout << "Client closed the connection." << std::endl;
					close(server.poll[i].fd);
					server.poll[i].fd = -1;
				} else { 				      // Actually received a message
					std::string info(buffer);
					std::cout << "[" << info << "]" << std::endl;
					//start parsing, reply
					//SCUFFED early reply to trick client into thinking its fully connected
					if (info.find("USER") != std::string::npos && (server.poll[i].fd & POLLOUT))
					{
						std::cout << "Responding to client" << std::endl;
						std::string join_response(":localhost 001 jeff :Welcome to the IRC server, jeff!\n");
						send(server.poll[i].fd, join_response.c_str(), join_response.length(), 0);
						std::string join_response(":localhost 001 jeff :Welcome to the IRC server, jeff!\n");
					}
				}
			}
		}
	}
	return (0);
}