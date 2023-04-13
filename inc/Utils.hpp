/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 01:45:40 by gborne            #+#    #+#             */
/*   Updated: 2023/04/13 13:41:04 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	UTILS_HPP
# define UTILS_HPP

# include <iostream>
# include <fstream>
# include <sstream>
# include <vector>
# include <cstring>
# include <cstdlib> //free()
# include <unistd.h> // get_current_dir_name()
# include <ctype.h> // isdigit()
# include <sys/param.h> // MAXPATHLEN
# include <sys/stat.h> // Vérif Dossier
# include <cerrno>

#define	RED			"\033[1;31m"
#define	GREEN		"\033[1;32m"
#define	YELLOW		"\033[1;33m"
#define	BLUE		"\033[1;34m"
#define	MAGENTA		"\033[1;35m"
#define	DEF			"\033[0m"

#define SUCCESS		"\033[1;32mSuccess \033[0m"
#define ERROR		"\033[1;31mError \033[0m"
#define WARNING		"\033[1;31mWarning \033[0m"
#define RECV		"\033[1;31m[<-] \033[0m"
#define	SEND		"\033[1;32m[->] \033[0m"
#define CONNECT		"\033[1;33mconnexion \033[0m"
#define DISCONNECT	"\033[1;33mdisconnection \033[0m"

namespace HTTP {

# define LINE_END	"\r\n"
# define BUFF_SIZE	8192

enum Code {

	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NO_CONTENT = 204,

	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	METHOD_NOT_ALLOWED = 405,
	REQUEST_TIMEOUT = 408,
	UNSUPORTED_MEDIA_TYPE = 415,

	INTERNAL_SERVER_ERROR = 500,
	BAD_GATEWAY = 502,
	SERVICE_UNAVAILABLE = 503,
	HTTP_VERSION_NOT_SUPPORTED = 505,
};

std::vector<std::string>	split ( const std::string & str, const std::string & delim = LINE_END );

std::string					itoa( const int & num );

bool						is_number( const std::string & s );

bool						file_exist( const std::string & path );

std::string					get_key( const std::string & line, const std::string & separator = ": ");

std::string					get_value( const std::string & line, const std::string & separator = ": ");

std::string					get_extension( const std::string & real_path );

std::string					get_current_dir( void );

std::string					read_file( const std::string & path );

void						create_file( const std::string & file_name, const std::string & content );

} // namespace HTTP

#endif
