
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

// #define PORT 8080
#define HTML_HEADER "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: "

bool escape = false;

int	error_exit(std::string msg)
{
	std::cout << msg << errno << std::endl;
	exit(EXIT_FAILURE);
}

void sig_handler(int signum)
{
    if (signum == SIGINT) {
		// exit(1);
		escape = true;
		exit(1);
    }
}

int	main(int ac, char **av)
{
	signal(SIGINT, sig_handler);

	if (ac != 3)
		error_exit("so bad ");
	(void)av;

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
		error_exit("Failed to create socket. errno: ");

    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(atoi(av[1]));

    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
		error_exit("Failed to bind to port. errno: ");

    if (listen(sockfd, 10) < 0)
		error_exit("Failed to listen on socket. errno: ");

	struct sockaddr_in clientAddress;
	socklen_t clientAddressLength = sizeof(clientAddress);

	int clientSocket = accept(sockfd, (struct sockaddr*)&clientAddress, &clientAddressLength);
	if (clientSocket == -1)
		error_exit("Failed to accept TOS? errno: ");

	//clientSocket is now the FD to write and read on for one user
    while (true)
	{
		char buffer[1024]; // A buffer to store incoming data
		ssize_t bytesRead; // Number of bytes read

		// Read data from the clientSocket into the buffer
		bytesRead = read(clientSocket, buffer, sizeof(buffer));
		std::string info(buffer);
		std::cout << info;

		if (bytesRead == -1) {
			// Handle error
			perror("read");
		} else if (bytesRead == 0) {
			// Connection closed by the client
			std::cout << "Client closed the connection." << std::endl;
		} else {
			// Process and handle the received data
			buffer[bytesRead] = '\0'; // Null-terminate the received data
			std::cout << "Received data from client: " << buffer << std::endl;
		}

		//DETECT SERVER MESSAGE AND RESPOND ????????????????? xddd


		// if (info.find("CAP LS") == 0)
		// {
		// 	std::string response = "CAP * LS :multi-prefix NICK #xd";
		// 	write (clientSocket, response.c_str(), response.length());
		// 	std::cout << "RESPONDING TO CAP LS!" << std::endl;
		// }


	}

    return (0);
}