/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   colours.hpp                                        :+:    :+:            */
/*                                                     +:+                    */
/*   By: shoddy <shoddy@student.codam.nl>             +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/05 15:46:29 by shoddy        #+#    #+#                 */
/*   Updated: 2024/03/05 15:46:30 by shoddy        ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOURS_HPP
# define COLOURS_HPP

// Colours //
# define RESET	"\1\33[0m\2"
# define END	"\1\33[0m\2\3"
# define END_LINE "\1\33[0m\2\n\3"

# define YELLOW	"\1\33[38;5;220m\2"			/* Yellow */
# define BLACK	"\1\33[38;5;232m\2"			/* Black */
# define ORANGE	"\1\33[38;5;202m\2"			/* Orange */
# define RED	"\1\33[38;5;196m\2"			/* Red */
# define PINK	"\1\33[38;5;198m\2"			/* Pink */
# define GREEN	"\1\33[38;5;28m\2"			/* Green */
# define BLUE	"\1\33[38;5;33m\2"			/* Blue */
# define L_BLUE	"\1\33[38;5;69m\2"			/* Light Blue */
# define D_BLUE	"\1\33[38;5;21m\2"			/* Dark Blue */
# define GREY	"\1\33[38;5;242m\2"			/* Grey */
# define PURPLE	"\1\33[38;5;92m\2"			/* Purple */
# define VIOLET	"\1\33[38;5;183m\2"			/* Violet */

// Simple Colours
# define SIMPLE_WHITE       "\1\033[37m\2"             /* White */
# define SIMPLE_BLACK       "\1\033[30m\2"             /* Black */
# define SIMPLE_RED         "\1\033[31m\2"             /* Red */
# define SIMPLE_MAGENTA     "\1\033[35m\2"             /* Magenta */
# define SIMPLE_BLUE        "\1\033[34m\2"             /* Blue */
# define SIMPLE_CYAN        "\1\033[36m\2"             /* Cyan */
# define SIMPLE_YELLOW      "\1\033[33m\2"             /* Yellow */
# define SIMPLE_GREEN       "\1\033[32m\2"             /* Green */

// Simple Bold Colours
# define SIMPLE_BOLDWHITE   "\1\033[1m\033[37m\2"      /* Bold White */
# define SIMPLE_BOLDBLACK   "\1\033[1m\033[30m\2"      /* Bold Black */
# define SIMPLE_BOLDRED     "\1\033[1m\033[31m\2"      /* Bold Red */
# define SIMPLE_BOLDGREEN   "\1\033[1m\033[32m\2"      /* Bold Green */
# define SIMPLE_BOLDYELLOW  "\1\033[1m\033[33m\2"      /* Bold Yellow */
# define SIMPLE_BOLDBLUE    "\1\033[1m\033[34m\2"      /* Bold Blue */
# define SIMPLE_BOLDMAGENTA "\1\033[1m\033[35m\2"      /* Bold Magenta */
# define SIMPLE_BOLDCYAN    "\1\033[1m\033[36m\2"      /* Bold Cyan */

#endif

