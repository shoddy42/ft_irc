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

class User
{
	public:
		User(int sock, pollfd poll_data, int id);
		User(const User &src);
		~User(void);
		
		std::string name;
		std::string nick;
		pollfd		&poll_data;
		int			&sock;
		int			_id; //todo: decide what should be private
		
	private:
		User(void);
		User &operator=(User const &src);
		
};


#endif
