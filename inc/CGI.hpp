/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/09 13:17:05 by gborne            #+#    #+#             */
/*   Updated: 2022/12/13 15:48:38 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
# define CGI_HPP

# include <iostream>
# include <cstdlib> //free(), exit()
# include <unistd.h> //execve()
# include <sys/types.h>
# include <sys/wait.h>

# include "ConfigServer.hpp"
# include "Request.hpp"
# include "Utils.hpp"

namespace HTTP {

class CGI {

public:

	CGI( void );

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
