/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 19:35:40 by gborne            #+#    #+#             */
/*   Updated: 2023/03/16 01:52:14 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "ConfigServer.hpp"
# include "Request.hpp"

# include <sstream>
# include <fstream>
# include <stdlib.h> // itoa()

# include "Utils.hpp"
# include "CGI.hpp"

namespace HTTP {

class Response {

public:

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

	std::string	_set_cookie( void );

	void		_construct( void );

	void		_construct_cgi( void );

	void		_construct_error( const int & code, const std::string & msg = std::string() );

	void		_construct_delete( void );

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
