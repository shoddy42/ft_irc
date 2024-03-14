/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Bot.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2024/03/05 15:44:15 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Bot.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Bot::Bot(std::string port, std::string password): _password(password),
	_botname(DEFAULT_NAME), _in_server(false), _in_channel(false)
{
	try
	{
		_port = std::stoi(port);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	

}

Bot::Bot(const Bot &src)
{
	if (this != &src)
		*this = src;
}

/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Bot::~Bot(void)
{
}

/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Bot &Bot::operator=(Bot const &src)
{ //todo:
	if (this == &src)
		return (*this);

	return (*this);
}

/*
** --------------------------------- METHODS ----------------------------------
*/

void Bot::start(std::string ip)
{

	// guard(_socket = socket(AF_INET, SOCK_STREAM, 0), "Could not create socket.");

	// sockaddr_in server_address;
	// server_address.sin_port = htons(_port);
	// server_address.sin_family = AF_INET;
	// server_address.sin_addr.s_addr = inet_addr(DEFAULT_IP);

    // // Connect to the server
    // if (connect(_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1) {
    //     perror("Connection failed");
    //     close(_socket);
    // }
    // std::cout << "Connected to the server successfully\n";


	login();
	// send(_socket, "PASS a\r\n", 10, 0);
	// std::string name = "USER bwrae bwre aebrw bewa bewa\r\n";
	// send(_socket, name.c_str(), name.length(), 0);
	// std::cout << "sending!\n";
}

void	Bot::login(void)
{
	if (!_in_server)
	{
		close(_socket);
		_socket = -1;
		guard(_socket = socket(AF_INET, SOCK_STREAM, 0), "Could not create socket.");

		sockaddr_in server_address;
		server_address.sin_port = htons(_port);
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = inet_addr(DEFAULT_IP);

		// Connect to the server
		if (connect(_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1) {
			perror("Connection failed");
			close(_socket);
		}
		_server.fd =_socket;
		_server.events = POLLIN | POLLOUT;
		std::cout << "Connected to the server successfully\n";
		_in_server = true;
	}
	if (!_in_channel)
	{
		std::string pass = "PASS " + _password;
		_responses.push(pass);

		std::string user = "USER " + _botname + " " + _botname + " BOT BOT :BOT";
		_responses.push(user);

		std::string join = "JOIN " + std::string(DEFAULT_CHANNEL) + " " + std::string(DEFAULT_CHANNEL_PASS);
		_responses.push(join);
		std::cout << "Connecting to channel " << DEFAULT_CHANNEL << END_LINE;
		_in_channel = true;
		// std::cout << "?: " << _responses.size() << END_LINE;
	}
}

void	Bot::respond(void)
{
	while (_responses.size() > 0)
	{
		std::string response = _responses.front();
		_responses.pop();
		if (response.empty())
			continue;
		std::cout << DEFAULT_NAME << " >> " << YELLOW << response << RESET << std::endl;
		response += "\r\n";
		send(_socket, response.c_str(), response.length(), 0);
	}
}

std::string Bot::receive(void)
{
	char	buffer[BUFFER_SIZE];
	ssize_t	bytes_read;

	bzero(buffer, BUFFER_SIZE);
	bytes_read = recv(_server.fd, buffer, BUFFER_SIZE, 0);
	if (bytes_read == -1)	  // Recv failed
		return ("");
	else if (bytes_read == 0) // Connection closed by the client
	{

	}
	else  				      // Actually received a message
		return (std::string(buffer));
	return ("");
}

void Bot::message_channel(std::string msg)
{
	std::string response = "PRIVMSG " + std::string(DEFAULT_CHANNEL);
	response += " " + msg;
	_responses.push(response);
}

void Bot::process(std::string packet)
{
	std::cout << "processing\n";
	if (packet.find("?") != std::string::npos)
	{
		int chance = rand() % 2;
		if (chance == 0)
			message_channel("Yes.");
		else
			message_channel("No.");
	}
}

void Bot::serve(void)
{
	poll(&_server, 1, POLL_TIMEOUT);

	// std::cout << "?2: " << _responses.size() << END_LINE;
	if (_server.revents == 0)
	{
		return;
	} //server hasnt sent anything
	else if (_server.revents & POLLHUP) //server disconnected
	{
		login();
	}
	else if (_server.revents & POLLIN) //client sent server a message
	{
		_unfinished_packet += receive();
		if (_unfinished_packet.find(std::string("\n")) == std::string::npos)
			std::cout << RED << "Received Unfinished packet [" << _unfinished_packet << "]" << std::endl << RESET;
		else
		{
			std::cout << BLUE << "Received [" << _unfinished_packet << "]" << std::endl << RESET;
			process(_unfinished_packet);
			_unfinished_packet.clear();
		}
		_server.revents = 0;
	}
	else if (_server.revents & POLLOUT) //client is ready for a response
	{
		respond();
	}
}

void Bot::stop(void)
{
	send(_socket, "QUIT\r\n", 6, 0);
	close(_socket);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

// getters



// setters


/* ************************************************************************** */