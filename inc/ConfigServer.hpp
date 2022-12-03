/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 16:20:08 by gborne            #+#    #+#             */
/*   Updated: 2022/12/03 04:46:53 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

# include <iostream>
# include <vector>

# include "message.h"

struct Location {

	std::string					name;
	std::vector<std::string>	methods;
	std::string					index;
	std::string					root;
	std::string					php;
	std::string					cgi;

	Location & operator=( Location const & rhs ) {
		this->name = rhs.name;
		this->methods = rhs.methods;
		this->index = rhs.index;
		this->root = rhs.root;
		this->php = rhs.php;
		this->cgi = rhs.cgi;
		return *this;
	}
};

class ConfigServer {

public:

	typedef std::vector<Location>			locations;
	typedef locations::iterator				iterator;
	typedef locations::const_iterator		const_iterator;

	// CANONICAL FORM

	ConfigServer( void );

	ConfigServer( std::string host, int port, std::string errorPath );

	ConfigServer( ConfigServer const & src );

	~ConfigServer();

	ConfigServer &	operator=( ConfigServer const & rhs );

	// ITERATORS

	iterator		begin( void );

	iterator		end( void );

	const_iterator	begin( void ) const;

	const_iterator	end( void ) const;

	// SETTERS

	void	setHost( std::string host );

	void	setPort( int port );

	void	setServerName( std::string serverName );

	void	setErrorPath( std::string errorPath );

	void	setBodyLimit( int bodyLimit );

	void	addLocation( std::string name, std::vector<std::string> methods,
		std::string index, std::string root, std::string php, std::string cgi );

	// GETTERS

	std::string	getHost( void ) const;

	int			getPort( void ) const;

	std::string	getServerName( void ) const;

	std::string	getErrorPath( void ) const;

	int			getBodyLimit( void ) const;

	locations	getLocations( void ) const;

private:

	std::string			_host;
	int					_port;
	std::string			_serverName;
	std::string			_errorPath;
	int					_bodyLimit;
	locations			_locations;

};

std::ostream &	operator<<( std::ostream & o, ConfigServer const & rhs );

#endif
