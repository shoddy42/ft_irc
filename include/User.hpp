/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   User.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2024/03/05 17:10:23 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef User_HPP
# define User_HPP
# include <string>
# include <poll.h>
# include <queue>

# define NULL_USER "" //dont change pls

class User
{
	private:
		std::queue<std::string> _responses;
		std::string	_username;
		std::string _nickname;
		std::string _realname;
		std::string _hostname;
		bool		_authenticated;
		int			_socket;

	public:
	//	constructors  //
	
		User(int sock);
		User(const User &src);
		User &operator=(User const &src);
		~User(void);

	//	methods  //
	
		void		add_response(std::string response);
		std::string	give_response(void);
		int			check_response(void);
		void		authenticate(void);

	//	getters/setters  //
	
		const std::string	&get_username(void);
		const std::string	&get_nickname(void);
		const std::string	&get_realname(void);
		const std::string	&get_hostname(void);
		const bool			&get_authenticated(void);
		const int			&get_socket(void);

		void		set_socket(int socket);
		void		set_nickname(std::string nick);
		void		set_username(std::string username);
		void		set_realname(std::string realname);
		void		set_hostname(std::string hostname);

	private:
		User(void);
};

#endif
