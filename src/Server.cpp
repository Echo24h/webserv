/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 16:20:48 by gborne            #+#    #+#             */
/*   Updated: 2022/11/29 17:55:16 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

Server::Server( void ) : _host("localhost"), _port(80), _errorPath("err/"), _bodyLimit(1980) {
	return ;
}

Server::Server( std::string host, int port, std::string errorPath ) : _bodyLimit(1980) {
	setHost(host);
	setPort(port);
	setErrorPath(errorPath);
	return ;
}

Server::Server( Server const & src ) {
	*this = src;
	return ;
}

Server::~Server( void ) {
	return ;
}

Server & Server::operator=( Server const & rhs ) {
	_host = rhs._host;
	_port = rhs._port;
	_serverName = rhs._serverName;
	_errorPath = rhs._errorPath;
	_bodyLimit = rhs._bodyLimit;
	_locations = rhs._locations;
	return *this;
}

// SETTERS

void	Server::setHost( std::string host ) {
	if (host.empty())
		throw std::invalid_argument("[Server.cpp] host is empty");
	_host = host;
	return ;
}

void	Server::setPort( int port ) {
	if (port < 1 || port > 65535)
		throw std::out_of_range("[Server.cpp] port must be between 1 and 65535 inclusive");
	_port = port;
	return ;
}

void	Server::setServerName( std::string serverName ) {
	_serverName = serverName;
	return ;
}

void	Server::setErrorPath( std::string errorPath ) {
	if (errorPath.empty())
		throw std::invalid_argument("[Server.cpp] error path is empty");
	_errorPath = errorPath;
	return ;
}

void	Server::setBodyLimit( int bodyLimit ) {
	if (bodyLimit < 1)
		throw std::invalid_argument("[Server.cpp] body must be larger than 1px");
	_bodyLimit = bodyLimit;
	return ;
}

void	Server::addLocation( std::string name, std::vector<std::string> methods,
std::string index, std::string root, std::string php, std::string cgi ) {

	Location newLoc;
	newLoc.name = name;
	newLoc.methods = methods;
	newLoc.index = index;
	newLoc.root = root;
	newLoc.php = php;
	newLoc.cgi = cgi;
	_locations.push_back(newLoc);
	return ;
}

// GETTERS

std::string			Server::getHost( void ) const { return _host; }

int					Server::getPort( void ) const { return _port; }

std::string			Server::getServerName( void ) const { return _serverName; }

std::string			Server::getErrorPath( void ) const { return _errorPath; }

int					Server::getBodyLimit( void ) const { return _bodyLimit; }

Server::locations	Server::getLocations( void ) const { return _locations; }

// STREAM

std::ostream &	operator<<( std::ostream & o, Server const & rhs ) {
	o << "host       = " << rhs.getHost() << std::endl;
	o << "port       = " << rhs.getPort() << std::endl;
	o << "serverName = " << rhs.getServerName() << std::endl;
	o << "errorPath  = " << rhs.getErrorPath() << std::endl;
	o << "bodyLimit  = " << rhs.getBodyLimit() << std::endl;
	Server::locations	loc = rhs.getLocations();
	Server::locations::const_iterator	it = loc.begin();
	Server::locations::const_iterator	ite = loc.end();
	while (it != ite) {
		o << "location   = " << (*it).name << std::endl;
		o << "	methods = ";
		std::vector<std::string>::const_iterator	it_m = (*it).methods.begin();
		std::vector<std::string>::const_iterator	ite_m = (*it).methods.end();
		while (it_m != ite_m) {
			o << *it_m << " ";
			it_m++;
		}
		o << std::endl;
		o << "	index   = " << (*it).index << std::endl;
		o << "	root    = " << (*it).root << std::endl;
		o << "	php     = " << (*it).php << std::endl;
		o << "	cgi     = " << (*it).cgi << std::endl;
		it++;
	}
	return o;
}
