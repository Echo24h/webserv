/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 16:20:08 by gborne            #+#    #+#             */
/*   Updated: 2022/11/29 14:44:12 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <vector>

struct Location {

	std::string					name;
	std::vector<std::string>	methods;
	std::string					index;
	std::string					root;
	std::string					php;
	std::string					cgi;

};

class Server {

public:

	typedef std::vector<Location>	locations;

	// CANONICAL FORM

	Server( void );

	Server( std::string host, int port, std::string errorPath );

	Server( Server const & src );

	~Server();

	Server &	operator=( Server const & rhs );

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

std::ostream &	operator<<( std::ostream & o, Server const & rhs );

#endif
