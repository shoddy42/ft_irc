/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   User.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/10/18 01:03:05 by root          ########   odam.nl         */
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
		User(int sock, int id);
		User(const User &src);
		~User(void);

		const int			get_socket(void);
		const std::string	get_name(void);
		const std::string	get_nick(void);

		void	set_socket(int socket);
		void	set_name(std::string);
		void	set_nick(std::string);
		
		
	private:
		User(void);
		User &operator=(User const &src);
		
		std::queue<std::string> _responses;
		std::string _name;
		std::string _nick;
		int			_socket;
		int			_id;
		bool		_authenticated;
};


#endif
