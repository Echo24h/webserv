/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 13:17:05 by gborne            #+#    #+#             */
/*   Updated: 2023/04/21 06:23:14 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <cstdlib> //free(), exit()
# include <unistd.h> //execve()
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <signal.h> // kill() for mac

# include "ConfigServer.hpp"
# include "Request.hpp"
# include "Utils.hpp"

namespace HTTP {

#define ERROR_EXEC_COUNT 30 // 1 = 0.1s, 10 = 1.0s

class CGI {

public:

	CGI( const ConfigServer * config, const Request * request );

	CGI( const CGI & src );

	~CGI();

	CGI &	operator=( const CGI & rhs );

	// GETTERS

	int			get_code( void ) const;

	std::string	get_type( void ) const;

	std::string	get_content( void ) const;

private:

	// FUNCTIONS

	void		_send_content( int fd, const std::string & content );

	std::string	_exec( void );

	std::string	_read( const int & fd ) const;

	void		_construct( void );

	char **		_generate_env( void ) const;

	const ConfigServer *	_config;
	const Request *			_request;

	int						_code;
	std::string				_type;
	std::string				_content;

};

} // namespace HTTP

#endif
