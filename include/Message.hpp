/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Message.hpp                                           :+:    :+:            */
/*                                                     +:+                    */
/*   By: wkonings <wkonings@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2023/07/19 13:21:59 by wkonings      #+#    #+#                 */
/*   Updated: 2023/09/05 18:43:26 by wkonings      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef Message_HPP
# define Message_HPP
# include <string>
# include "../include/User.hpp"

//todo: consider deleting the entire class.
// time sent is useless and we can just pass the sender to relevant functions
class Message
{
	public:
		// Message(void);
		Message(User &sender, std::string text);
		Message(const Message &src);
		~Message(void);
		Message &operator=(Message const &src);

		const std::string	&get_text(void);
		const User			&get_sender(void);
		
	private:
		std::string &_text;		//tmp
		User		&_sender;
		int			time_sent;	//tmp
};


#endif
