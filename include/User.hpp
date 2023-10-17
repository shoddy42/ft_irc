/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   User.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/10/17 14:00:12 by root          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef User_HPP
# define User_HPP
# include <string>
# include <poll.h>

class User
{
	public:
		User(void);
		User(const User &src);
		~User(void);
		User &operator=(User const &src);
		std::string name;
		std::string nick;
		pollfd		poll_data;
		int			sock;
		
	private:
		
};


#endif
