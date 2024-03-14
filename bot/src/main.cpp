/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.cpp                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:51 by wkonings      #+#    #+#                 */
/*   Updated: 2024/03/05 15:44:15 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Bot.hpp"

bool escape;

/**
 * @brief if n == -1, will exit the program, writing error_msg.
 * 		  use instead of small if blocks.
 * 
 * @return @b int n
 */
int guard(int n, std::string error_msg)
{
	if (n == -1)
		escape = true;
	return (n);
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

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
        return 1;
    }
	signal(SIGINT, sig_handler); //Signals to make ctrl+c quit program as expected
    srand(time(NULL)); //Set a seed based on time for future rand() calls.
    escape = false;
    Bot irc_bot(argv[1], argv[2]);
    irc_bot.start(DEFAULT_IP);
    // Add further functionality here
    while(escape == false)
    {
        sleep (1);
        irc_bot.serve();
    }
    irc_bot.stop();
    return 0;
}
