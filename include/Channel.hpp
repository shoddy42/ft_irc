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

class Channel
{
	public:
		Channel(void);
		Channel(const Channel &src);
		~Channel(void);
		Channel &operator=(Channel const &src);
		
	private:
		int users; //tmp
		// Message messages[];
		
};


#endif
