/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 19:35:40 by gborne            #+#    #+#             */
/*   Updated: 2022/12/05 20:31:25 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ConfigServer.hpp"
# include "Request.hpp"

# include <sstream>
# include <fstream>
# include <stdlib.h> // itoa()

namespace HTTP {

enum Code {
	OK = 200,
	CREATED = 201,
	ACCEPTED = 202,
	NO_CONTENT = 203,
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

	Response( ConfigServer * config, const Request & request );

	Response( const Response & src );

	~Response();

	Response & operator=( const Response & rhs );

	std::string	to_string( void ) const;

	// GETTERS

	std::string	file_path( void ) const;

	std::string	file_type( void ) const;

	int	code( void ) const;

private:

	// Parsing

	void		_serialize( const Request & request );

	std::string	_get_file_path( const std::string & location, const std::string & method ) const;

	std::string	_get_file_type( const std::string & file_path ) const;

	// Methods

	void	_get( const std::string & location );

	void	_post( const std::string & location );

	void	_del( const std::string & location );

	// to_string()

	std::string	_format( const std::string & path ) const;

	std::string	_content( const std::string & path ) const;

	// Members

	std::string		_file_path;
	std::string		_file_type;
	int				_code;
	ConfigServer *	_config;

};

std::ostream &	operator<<( std::ostream & o, Response const & rhs );

} // namespace HTTP

#endif
