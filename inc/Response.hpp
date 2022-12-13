/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 19:35:40 by gborne            #+#    #+#             */
/*   Updated: 2022/12/13 14:52:05 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ConfigServer.hpp"
# include "Request.hpp"

# include <sstream>
# include <fstream>
# include <stdlib.h> // itoa()

# include "CGI.hpp"

namespace HTTP {

enum Code {
	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NO_CONTENT = 204,
	BAD_REQUEST = 400,
	FORBIDDEN = 403,
	NOT_FOUND = 404,
	REQUEST_TIMEOUT = 408,
	INTERNAL_SERVER_ERROR = 500,
	BAD_GATEWAY = 502,
	SERVICE_UNAVAILABLE = 503,
};

class Response {

public:

	Response( void );

	Response( const ConfigServer * config, const Request * request );

	Response( const Response & src );

	~Response();

	Response & operator=( const Response & rhs );

	// GETTERS

	int			get_code( void ) const;

	std::string	get_final_path( void ) const;

	std::string	get_type( void ) const;

	// FUNCTIONS

	std::string	to_string( void ) const;

private:

	void		_construct( void );

	void		_construct_cgi( void );

	std::string	_construct_content( const std::string & path ) const;

	const ConfigServer *	_config;
	const Request *			_request;

	int						_code;
	std::string				_final_path;
	std::string				_type;
	std::string				_content;
};

std::ostream &	operator<<( std::ostream & o, Response const & rhs );

} // namespace HTTP

#endif
