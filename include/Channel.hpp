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
# include "User.hpp"
# include <queue>
# include <list>
# include <string>
# include <iostream>
# include "../include/User.hpp"
# include "../include/Server.hpp"
# include "../include/print.hpp"

# define DEFAULT_TOPIC ":Welcome to the channel!"

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
		// std::vector<Message> _message_log;
		std::vector<std::string> _message_log;
		std::vector<User *>  _operator_list;
		std::string			 _name;
		std::string			 _topic;
		std::string			_password;
		int					_user_limit;
		bool				_invite_only;
		Server				 &_server;

	
	public:
		// Channel(void);
		std::list<User *>	_user_list;
		Channel(std::string channel_name, Server &server);
		Channel(const Channel &src);
		~Channel(void);
		Channel &operator=(Channel const &src);

		const std::string	&get_name(void);
		const std::string 	&get_topic(void);
		void 				set_topic(std::string topic);

		bool 	is_operator(User &user);

		void	add_user(User &user);
		void	remove_user(User &user);
		void	add_operator(User &user);

		void	send_message(std::string &message, User &sender);
};


#endif