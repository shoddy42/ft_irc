
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
		write(clientSocket, "XD", 2);
		// sleep(5);
	}

    return (0);
}