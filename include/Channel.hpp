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
# include "../include/Server.hpp"
# include "../include/User.hpp"
# include "../include/colours.hpp"
# include <iostream>
# include <string>
// # include <queue>
# include <list>

// default settings for channels.
# define DEFAULT_TOPIC ":Welcome to the channel!"
# define DEFAULT_RESTRICT true

class Server;

class Channel
{
	private:
		std::vector<std::string> _message_log;
		std::list<User *>	_operator_list;
		std::list<User *>	_invited_list;
		std::list<User *>	_user_list;
		std::string			_name;
		std::string			_topic; //todo: maybe store more than just the topic, like topic_who and topic_when for extra style points?
	
		std::string			_password;
		int					_creation_time;
		int					_user_limit;
		bool				_password_required;
		bool				_topic_restricted;
		bool				_invite_only;
		Server				 &_server;
	
	public:
	//	constructors  //

		Channel(std::string channel_name, Server &server);
		Channel(const Channel &src);
		Channel &operator=(Channel const &src);
		~Channel(void);

	//	methods  //

		void	send_message(std::string &message, User &sender);
		void	send_notice(std::string &message);
		void	send_channel_info(User &user);

		void	add_user(User &user);
		void	add_invited(User &user);
		void	add_operator(User &user);
	
		bool	remove_user(User &user, std::string reason); //returns if the channel was deleted or not.
		void	remove_operator(User &user);
		void	remove_invited(User &user);
		void	remove_password(void);
		void	kick_user(User &user);

		void	mode(User &caller);
		void	who(User &caller);

	//	getters/setters  //

		const std::string	&get_name(void);
		const std::string 	&get_topic(void);
		const std::string	&get_password(void);
		// const std::string	&get_creation_time(void);

		void 	set_topic(std::string topic);
		void 	set_topic_restriction(bool deny_plebs);
		void	set_password(std::string password);
		void	set_invite_only(bool is_private);
		void	set_user_limit(int limit);

		bool	has_password(void);
		bool	is_topic_restricted(void);
		bool 	is_operator(User &user);
		bool 	is_invited(User &user);
		bool 	is_user(User &user);
	
	private:
		Channel(void);
};


#endif