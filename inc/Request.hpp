/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 00:10:36 by gborne            #+#    #+#             */
/*   Updated: 2022/12/05 18:32:17 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

// https://codereview.stackexchange.com/questions/205704/very-basic-c-http-parser

# include "Utils.hpp"

# include <iostream>
# include <map>

# include <sys/socket.h>
# include <string.h>
# include <errno.h>

namespace HTTP {

class Request {

public:

	Request( void );

	Request( const std::string & buff );

	Request( int client_socket );

	Request( const Request & src );

	~Request();

	Request	& operator=( Request const & rhs );

	static Request		rec( int client_socket );

	std::string	to_string( int client_socket ) const;

	// GETTERS

	std::string	version( void ) const;

	std::string	method( void ) const;

	std::string	location( void ) const;

	std::map<std::string, std::string>	ressource( void ) const;

private:

	void	_header( const std::string & line );

	void	_serialize( const std::string & buff );

	std::string							_method;
	std::string							_location;
	std::string							_version;
	std::map<std::string, std::string>	_ressource;

};

std::ostream &	operator<<( std::ostream & o, Request const & rhs );

} // namesapce HTTP

#endif
