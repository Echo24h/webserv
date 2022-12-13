/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 00:10:36 by gborne            #+#    #+#             */
/*   Updated: 2022/12/12 02:10:00 by gborne           ###   ########.fr       */
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

# include "ConfigServer.hpp"

namespace HTTP {

class Request {

public:

	typedef	std::map<std::string, std::string>	ressources;

	Request( void );

	Request( const ConfigServer * config, const int & client_socket );

	Request( const Request & src );

	~Request();

	Request	& operator=( const Request & rhs );

	// GETTERS

	std::string	get_method( void ) const;

	std::string	get_virtual_path( void ) const;

	std::string	get_real_path( void ) const;

	std::string	get_cgi( void ) const;

	std::string	get_query( void ) const;

	std::string	get_client_ip( void ) const;

	std::string	get_version( void ) const;

	std::string	get_ressource( const std::string & key ) const ;

private:

	std::string	_to_string( const int & client_socket ) const;

	std::string	_get_key( const std::string & line ) const;

	std::string	_get_value( const std::string & line ) const;

	std::string	_get_real_path( const std::string & virtual_path, const std::string & method ) const;

	void		_construct_header( const std::string & line );

	void		_construct( const std::string & buff );

	std::string	_method;
	std::string	_virtual_path;
	std::string	_real_path;
	std::string	_cgi;
	std::string	_query;
	std::string	_client_ip;
	std::string	_version;
	ressources	_ressources;

	const ConfigServer * _config;

};

std::ostream &	operator<<( std::ostream & o, Request const & rhs );

} // namesapce HTTP

#endif
