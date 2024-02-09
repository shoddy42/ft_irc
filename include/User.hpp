/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   User.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2024/02/09 16:50:17 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef User_HPP
# define User_HPP
# include <string>
# include <poll.h>
# include <queue>

class User
{
	public:
		User(int sock);
		User(const User &src);
		User &operator=(User const &src);
		~User(void);

		void		add_response(std::string response);
		std::string	give_response(void);
		int			check_response(void);

		const int			&get_socket(void);
		const bool			&get_authenticated(void);
		const std::string	&get_username(void);
		const std::string	&get_nickname(void);

		void		set_socket(int socket);
		void		set_username(std::string);
		void		set_nickname(std::string);
		void		authenticate(void);

	private:
		User(void);

		std::queue<std::string> _responses;
		std::string	_username;
		std::string _nickname;
		int			_socket;
		bool		_authenticated;
};

#endif
