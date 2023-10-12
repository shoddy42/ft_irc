
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

#define DEFAULT_PORT 6667
#define BUFFER_SIZE	1024
#define HTML_HEADER "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "

bool escape = false;

int	error_exit(std::string msg)
{
	std::cout << msg << errno << std::endl;
	exit(EXIT_FAILURE);
}

/**
 * @brief Just makes ctrl+c quit the program when used with Signal()
 * 
 * @param signum automatically gets set by signals.
 * 
 * @todo maybe attempt to use the "escape" global to quit?
 */
void sig_handler(int signum)
{
    if (signum == SIGINT) {
		escape = true;
		// exit(1);
    }
}

/**
 * @brief Creates a new socket to listen on port
 * 
 * @param port Port to bind socket to. Use 0 for default port.
 * @return int (the socket FD)
 */
int	create_socket(int port)
{
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
		error_exit("Failed to create socket. errno: ");

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;

	if (port == 0)
		sockaddr.sin_port = htons(DEFAULT_PORT);
	else
		sockaddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
		error_exit("Failed to bind to port. errno: ");

    if (listen(sockfd, 10) < 0)
		error_exit("Failed to listen on socket. errno: ");

	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);

	int clientSocket = accept(sockfd, (struct sockaddr*)&clientAddress, &clientAddressLength);
	if (clientSocket == -1)
		error_exit("Failed to accept TOS? errno: ");
	
	return (clientSocket);
}

int	main(int ac, char **av)
{
	//Signals to make ctrl+c to escape from "Accept" being stuck if no client connects.
	signal(SIGINT, sig_handler);

	int clientSocket;
	if (ac == 3)
		clientSocket = create_socket(atoi(av[1]));
	else
		clientSocket = create_socket(0);

	//clientSocket is now the FD to write and read on for one user

    while (escape == false)
	{
		char buffer[BUFFER_SIZE]; // A buffer to store incoming data
		ssize_t bytesRead = 0; // Number of bytes read

		// Read data from the clientSocket into the buffer
		if (bytesRead == 0)
			bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
		std::string info(buffer);
		std::cout << info;

		if (bytesRead == -1) {
			// Handle error
			perror("read");
		} else if (bytesRead == 0) {
			// Connection closed by the client
			std::cout << "Client closed the connection." << std::endl;
			exit (0); //remove this later, just needed to prevent the giga spam rn
		} else {
			// Process and handle the received data
			buffer[bytesRead] = '\0'; // null terminate the received data
			std::cout << "Received data from client: " << buffer << std::endl;
		}
	}
    return (0);
}