/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   User.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/09/05 18:55:11 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */



#ifndef User_HPP
# define User_HPP
# include <string>

class User
{
	public:
		User(void);
		User(const User &src);
		~User(void);
		User &operator=(User const &src);
		
	private:
		std::string name;
		std::string nick;

};


#endif
