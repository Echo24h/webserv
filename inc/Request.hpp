/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 00:10:36 by gborne            #+#    #+#             */
/*   Updated: 2023/04/21 09:02:55 by gborne           ###   ########.fr       */
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
# include "Utils.hpp"

namespace HTTP {

class Request {

public:

	typedef	std::map<std::string, std::string>	ressources;

	Request( ConfigServer * config, const int & client_socket );

	Request( const Request & src );

	~Request();

	Request	& operator=( const Request & rhs );

	// GETTERS

	std::string	get_method( void ) const;

	ConfigLocation	* get_location( void ) const;

	std::string	get_virtual_path( void ) const;

	std::string	get_real_path( void ) const;

	std::string	get_cgi( void ) const;

	bool		is_cgi( void ) const;

	std::string	get_query( void ) const;

	std::string	get_client_ip( void ) const;

	std::string	get_version( void ) const;

	std::string	get_ressource( const std::string & key ) const ;

	std::string	get_content( void ) const;

	std::string get_full_request( void ) const;

private:

	void		_construct_first_line( const std::string & line );

	void 		_construct_header( const std::string & buff );

	void		_construct( const int & client_socket );

	std::string	_method;
	std::string	_virtual_path;
	std::string	_real_path;
	std::string	_cgi;
	std::string	_query;
	std::string	_client_ip;
	std::string	_version;
	ressources	_ressources;
	std::string	_content;
	std::string _full_request;

	ConfigServer *	_config;
	ConfigLocation *		_loc;

};

std::ostream &	operator<<( std::ostream & o, Request const & rhs );

} // namesapce HTTP

#endif
