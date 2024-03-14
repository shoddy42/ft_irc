/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Bot.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2024/03/15 00:56:49 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Bot.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Bot::Bot(std::string port, std::string password): _password(password), _in_server(false), _in_channel(false)
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

void Bot::execute_advanced_ai(std::string packet)
{
	// Check if the message is received within a certain time window after joining
	std::cout << "test: [" << (mask() + " JOIN :" + std::string(BOT_CHANNEL)) << "]\n";

	if (packet.find(mask() + " KICK :" + std::string(BOT_CHANNEL)) != std::string::npos)
	{
		std::cout << RED << "Kicked from channel" << END_LINE;
		_in_channel = false;
	}
	else if (packet.find("?") != std::string::npos)
	{
		int chance = rand() % 2;
		if (chance == 0)
			message_channel("Yes.");
		else
			message_channel("No.");
	}
	else if (packet.find("GPT") != std::string::npos || packet.find("gpt") != std::string::npos)
	{
		// std::string gpt_answer = ask_question("Once upon a time");
		// std::cout << GREEN << gpt_answer << END_LINE; 
		// message_channel(gpt_answer);
	}
	else if (packet.find("noob") != std::string::npos)
	{
		message_channel("no u");
	}
}

void	Bot::login(void)
{
	if (!_in_server)
	{
		if (_socket > 0)
		{
			close(_socket);
			_socket = -1;
		}
		guard(_socket = socket(AF_INET, SOCK_STREAM, 0), "Could not create socket.");

		sockaddr_in server_address;
		server_address.sin_port = htons(_port);
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = inet_addr(BOT_IP);

		if (connect(_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) == -1) {
			perror("Connection failed");
			close(_socket);
		}
		_server.fd =_socket;
		_server.events = POLLIN | POLLOUT;
		std::cout << "Connected to the server successfully\n";
		std::string pass = "PASS " + _password;
		_responses.push(pass);
		
		std::string user = "USER " + std::string(BOT_USER) + " " + std::string(BOT_NAME) + " " + std::string(BOT_HOST) + " :" + std::string(BOT_REAL);
		_responses.push(user);

		_in_server = true;
	}
	if (!_in_channel)
	{
		std::string join = "JOIN " + std::string(BOT_CHANNEL) + " " + std::string(BOT_CHANNEL_PASS);
		_responses.push(join);
		std::cout << "Connecting to channel " << BOT_CHANNEL << END_LINE;
		message_channel(std::string(BOT_NAME) + " has joined to answer all your questions.");
		_in_channel = true;
		_join_time = std::chrono::steady_clock::now();
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
		std::cout << BOT_NAME << " >> " << YELLOW << response << RESET << std::endl;
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

std::string Bot::mask(void)
{
	return (":" + std::string(BOT_NAME) + "!" + std::string(BOT_USER) + "@" + std::string(BOT_HOST));
}


void Bot::message_channel(std::string msg)
{
	std::string response = "PRIVMSG " + std::string(BOT_CHANNEL);
	response += " " + msg;
	_responses.push(response);
}

// Hugging Face API endpoint for text generation
// const std::string API_URL = "https://api-inference.huggingface.co/models/gpt2";


void Bot::process(std::string buffer)
{
	if (buffer.empty())
		return;
	auto now = std::chrono::steady_clock::now();
	auto time_elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - _join_time).count();
	if (time_elapsed < JOIN_MESSAGE_WINDOW)
	{
		std::cout << PURPLE << "Disregarding" << END_LINE;
		_unfinished_packet.clear();
		return;
	}
	std::stringstream ss(buffer);
	std::string line;
	while (std::getline(ss, line, '\n'))
	{
		if (line.empty())
			continue;
		std::cout << "Server << " << BLUE << line << RESET << std::endl;
		execute_advanced_ai(line);
	}
	_unfinished_packet.clear();
}

void Bot::serve(void)
{
	poll(&_server, 1, POLL_TIMEOUT);

	if (_server.revents == 0) //server hasnt sent anything
		return;
	else if (_server.revents & POLLHUP || _in_server == false || _in_channel == false) //disconnected
		login();
	else if (_server.revents & POLLIN) //server sent a message
	{
		_unfinished_packet += receive();
		if (_unfinished_packet.find(std::string("\n")) == std::string::npos)
			std::cout << RED << "Received Unfinished packet [" << _unfinished_packet << "]" << std::endl << RESET;
		else
			process(_unfinished_packet);
		_server.revents = 0;
	}
	else if (_server.revents & POLLOUT) //server is ready for a response
		respond();
}

void Bot::stop(void)
{
	send(_socket, "QUIT\r\n", 6, 0);
	_unfinished_packet.clear();
	// _responses.c.clear();
	close(_socket);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

// getters



// setters


/* ************************************************************************** */