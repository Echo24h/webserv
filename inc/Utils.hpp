/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 01:45:40 by gborne            #+#    #+#             */
/*   Updated: 2022/12/05 19:47:34 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <fstream>
# include <vector>

#define	RED			"\033[1;31m"
#define	GREEN		"\033[1;32m"
#define	YELLOW		"\033[1;33m"
#define	BLUE		"\033[1;34m"
#define	MAGENTA		"\033[1;35m"
#define	DEF			"\033[0m"

#define ERROR		"\033[1;31mError \033[0m"
#define RECV		"\033[1;31m[<-] \033[0m"
#define	SEND		"\033[1;32m[->] \033[0m"
#define CONNECT		"\033[1;33mconnexion \033[0m"
#define DISCONNECT	"\033[1;33mdisconnection \033[0m"

namespace HTTP {

# define LINE_END	"\r\n"
# define BUFF_SIZE	8192

std::vector<std::string>	split ( const std::string & str, const std::string & delim = LINE_END );

std::string	get_key( const std::string & line );

std::string	get_value( const std::string & line );

bool	file_exist( const std::string & path );


} // namespace HTTP

#endif
