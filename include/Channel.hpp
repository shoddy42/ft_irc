/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Channel.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/09/05 18:43:26 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef Channel_HPP
# define Channel_HPP
# include "Message.hpp"
# include "User.hpp"
# include <queue>
# include <string>
# include <iostream>
# include "../include/User.hpp"
# include "../include/Server.hpp"
# include "../include/print.hpp"

// typedef struct s_user_list
// {
// 	User		*user;
// 	s_user_list	*next;
// } t_user_list;

// typedef struct s_message_list
// {
// 	Message			*message;
// 	s_message_list	*next;
// } t_message_list;

class Server;

class Channel
{
	private:
		std::vector<Message> _message_log;
		std::vector<User *>  _operator_list;
		std::string			 _name;
		Server				 &_server;
	
	public:
		// Channel(void);
		std::vector<User *>	_user_list;
		Channel(std::string channel_name, Server &server);
		Channel(const Channel &src);
		~Channel(void);
		Channel &operator=(Channel const &src);

		const std::string &get_name(void);

		void	add_user(User &user);
		void	add_operator(User &user);

		void	add_message(Message &message); //todo: remove add_msg and send_text
		void	send_text(std::string text);

		void	send_message(Message &message);
};


#endif