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

typedef struct s_user_list
{
	User		*user;
	s_user_list	*next;
} t_user_list;

typedef struct s_message_list
{
	Message			*message;
	s_message_list	*next;
} t_message_list;

class Channel
{
	private:

		t_user_list		_userList;
		t_message_list	_channelLog;
		Message			_newMessage;
	
	public:
		Channel(void);
		Channel(const Channel &src);
		~Channel(void);
		Channel &operator=(Channel const &src);


};


#endif