/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Bot.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2024/03/15 02:38:33 by shoddy        ########   odam.nl         */
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

static std::string to_lowercase(const std::string& str)
{
    std::string ret;
    for (char c : str)
        ret += std::tolower(c);
    return (ret);
}

/**
 * @brief simply add another else if block and use simple string parsing for additional features.
 * @param packet the entire string received by the server, but will made made all lower case.
 */
void Bot::execute_advanced_ai(std::string packet)
{
	std::cout << "test   << " << PINK << (mask() + " mode " + to_lowercase(BOT_CHANNEL) + " +o") << END_LINE;
	packet = to_lowercase(packet);
	if (packet.find(mask() + " kick " + to_lowercase(BOT_CHANNEL)) != std::string::npos)
	{
		std::cout << RED << "Kicked from channel" << END_LINE;
		_in_channel = false;
		_was_kicked = true;
	}
	else if (packet.find(":you are not in the channel " + to_lowercase(BOT_CHANNEL)) != std::string::npos)
		_in_channel = false;
	else if (packet.find(" mode " + to_lowercase(BOT_CHANNEL) + " +o :" + to_lowercase(BOT_NAME)) != std::string::npos)
	{
		std::cout << GREEN << "Modded in channel!" << END_LINE;
		_was_kicked = false;
		_was_modded = true;
		message_channel("Thanks!");
	}
	else if (packet.find(" mode " + to_lowercase(BOT_CHANNEL) + " -o :" + to_lowercase(BOT_NAME)) != std::string::npos)
	{
		std::cout << RED << "Unmodded in channel!" << END_LINE;
		_was_kicked = true;
		_was_modded = false;
		message_channel("What did I do wrong? >:[");
	}
	else if (packet.find("sorry") != std::string::npos)
	{
		message_channel("okay, no problem. I'll answer questions again :)");
		_was_kicked = false;
	}
	else if (packet.find("gpt") != std::string::npos)
		message_channel("I'm better");
	else if (packet.find("fact") != std::string::npos || packet.find("facts") != std::string::npos)
	{
		int chance = rand() % 8;
		if (_was_kicked)
			message_channel("You're annoying.");
		else if (chance == 0)
			message_channel("The Eiffel Tower can be 15 cm taller during the summer.");
		else if (chance == 1)
			message_channel("Octopuses have three hearts.");
		else if (chance == 2)
			message_channel("A group of flamingos is called a \"flamboyance.\"");
		else if (chance == 3)
			message_channel("Honey never spoils.");
		else if (chance == 4)
			message_channel("The shortest war in history lasted only 38 minutes.");
		else if (chance == 5)
			message_channel("The unicorn is the national animal of Scotland.");
		else if (chance == 6)
			message_channel("The world's oldest known recipe is for beer.");
		else if (chance == 7)
			message_channel("The human brain is more active during sleep than during the day when awake.");
	}
	else if (packet.find("hey") != std::string::npos || packet.find("hi") != std::string::npos || packet.find("hello") != std::string::npos)
	{
		int chance = rand() % 2;
		if (_was_kicked)
			message_channel("Bye.");
		else if (chance == 0)
			message_channel("Hey");
		else if (chance == 1)
			message_channel("Hello");
	}
	else if (packet.find("rock") != std::string::npos || packet.find("paper") != std::string::npos || packet.find("scissors") != std::string::npos)
	{
		int chance = rand() % 4;
		if (_was_kicked)
			return;
		else if (chance == 0)
			message_channel("Rock!");
		else if (chance == 1)
			message_channel("Paper!");
		else if (chance == 2)
			message_channel("Scissors!");
		else if (chance == 2)
			message_channel("Shotgun!");
	}
	else if (packet.find("?") != std::string::npos)
	{
		int chance = rand() % 10;
		if (_was_kicked)
			message_channel("I don't want to answer questions, unless you say sorry.");
		else if (chance >= 0 && chance <= 3)
			message_channel("Yes.");
		else if (chance >= 4 && chance <= 7)
			message_channel("No.");
		else if (chance == 8)
			message_channel("¯\\_(ツ)_/¯");
		else if (chance == 9)
			message_channel("Maybe...");
	}
	else if (packet.find("quantum") != std::string::npos)
		message_channel("whats up?");
	else
	{
		int chance = rand() % 100;
		if (chance == 0)
			message_channel("can i have op pls?");
		else if (chance == 1)
			message_channel("please dont kick me");
		else if (chance == 2)
			message_channel("ask me a question!");
		else if (chance == 3)
			message_channel("play rock paper scissors with me");
		else if (chance == 4)
			message_channel("ask me for a fact");
	}
}

/**
 * @brief Connects to server, and to channel if needed.
 */
void	Bot::login(void)
{
	if (!_in_server)
	{
		if (_socket > 0)
		{
			close(_socket);
			_socket = -1;
		}
		_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (_socket < 0)
		{
			perror("Could not create socket.");
			return;
		}
		sockaddr_in server_address;
		server_address.sin_port = htons(_port);
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = inet_addr(BOT_IP);

		if (connect(_socket, reinterpret_cast<sockaddr*>(&server_address), sizeof(server_address)) < 0)
		{
			perror("Connection failed");
			close(_socket);
			return;
		}

		_server.fd =_socket;
		_server.events = POLLIN | POLLOUT;
		std::cout << "Connected to the server successfully\n";
		std::string pass = "PASS " + _password;
		_responses.push(pass);
		
		std::string user = "USER " + std::string(BOT_USER) + " " + std::string(BOT_NAME) + " " + std::string(BOT_HOST) + " :" + std::string(BOT_REAL);
		_responses.push(user);

		_was_kicked = false;
		_in_server = true;
	}
	if (!_in_channel)
	{
		std::string join = "JOIN " + std::string(BOT_CHANNEL) + " " + std::string(BOT_CHANNEL_PASS);
		_responses.push(join);
		std::cout << "Connecting to channel " << BOT_CHANNEL << END_LINE;
		if (_was_kicked)
			message_channel(std::string(BOT_NAME) + " I can't believe you kicked me... ");
		else		
			message_channel(std::string(BOT_NAME) + " has joined to answer all your questions.");
		_in_channel = true;
		_was_modded = false;
		_disregarding_notice = false;
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

/**
 * @brief Returns the mask the server sends to signify it's for the bot IN LOWER CASE!!!.
 * 		  if find(mask()) != npos), its almost definitely for the bot.
 */
std::string Bot::mask(void)
{
	return (":" + to_lowercase(BOT_NAME) + "!" + to_lowercase(BOT_USER) + "@" + to_lowercase(BOT_HOST));
}

/**
 * @brief Adds "PRIVMSG @b (channel_name) " before your message.
 */
void Bot::message_channel(std::string msg)
{
	std::string response = "PRIVMSG " + std::string(BOT_CHANNEL);
	response += " " + msg;
	_responses.push(response);
}

/**
 * @brief splits string into smaller strings to feed to the insane ai
 */
void Bot::process(std::string buffer)
{
	if (buffer.empty())
		return;

	// Check if the message is received within a certain time window after joining, to avoid responding to history.
	auto now = std::chrono::steady_clock::now();
	auto time_elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - _join_time).count();
	if (time_elapsed < JOIN_MESSAGE_WINDOW)
	{
		if (_disregarding_notice == false)
		{
			std::cout << PURPLE << "Disregarding for " << JOIN_MESSAGE_WINDOW - time_elapsed << " seconds" << END_LINE;
			_disregarding_notice = true;
		}
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
	close(_socket);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

/* ************************************************************************** */