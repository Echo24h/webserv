/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 16:20:48 by gborne            #+#    #+#             */
/*   Updated: 2023/04/12 18:16:11 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ConfigServer.hpp"

namespace HTTP {

ConfigServer::ConfigServer( void ) : _host("localhost"), _port(80), _body_limit(1980) {
	return ;
}

ConfigServer::ConfigServer( const std::string & host, const int & port, const std::string & error_path ) : _body_limit(1980) {
	set_host(host);
	set_port(port);
	set_error_path(error_path);
	return ;
}

ConfigServer::ConfigServer( ConfigServer const & src ) {
	*this = src;
	return ;
}

ConfigServer::~ConfigServer( void ) {
	return ;
}

ConfigServer & ConfigServer::operator=( ConfigServer const & rhs ) {
	_host = rhs._host;
	_port = rhs._port;
	_server_name = rhs._server_name;
	_error_path = rhs._error_path;
	_body_limit = rhs._body_limit;
	_locations = rhs._locations;
	return *this;
}

// ITERATORS

ConfigServer::iterator ConfigServer::begin( void ) {
	return _locations.begin();
}

ConfigServer::iterator ConfigServer::end( void ) {
	return _locations.end();
}

ConfigServer::const_iterator ConfigServer::begin( void ) const {
	return _locations.begin();
}

ConfigServer::const_iterator ConfigServer::end( void ) const {
	return _locations.end();
}

// SETTERS

void	ConfigServer::set_host( const std::string & host ) {
	if (host.empty())
		throw std::invalid_argument("[ConfigServer.cpp] host is empty");
	if (_host == "localhost")
		_host = "127.0.0.1";
	else
		_host = host;
	return ;
}

void	ConfigServer::set_port( const int & port ) {
	if (port < 1 || port > 65535)
		throw std::out_of_range("[ConfigServer.cpp] port must be between 1 and 65535 inclusive");
	_port = port;
	return ;
}

void	ConfigServer::set_server_name( const std::string & server_name ) {
	_server_name = server_name;
	return ;
}

void	ConfigServer::set_error_path( const std::string & error_path ) {
	_error_path = error_path;
	return ;
}

void	ConfigServer::set_body_limit( const std::string & body_limit ) {

	char		extension = *body_limit.rbegin();
	std::string	num;

	if (extension == 'M' || extension == 'K')
		num = body_limit.substr(0, body_limit.size() - 1);
	else
		num = body_limit.substr(0, body_limit.size());

	if (num.empty() || !is_number(num))
		throw std::invalid_argument("[ConfigServer.cpp] wrong body_limit");

	if (extension == 'M')
		_body_limit = atoi(num.c_str()) * 1000000;
	else if (extension == 'K')
		_body_limit = atoi(num.c_str()) * 1000;

	if (_body_limit < 2)
		throw std::invalid_argument("[ConfigServer.cpp] body must be larger than 1Ko");
	//std::cout << _body_limit << std::endl;
	return ;
}

void	ConfigServer::new_location( ConfigServer::location & location ) {
	_locations.push_back(location);
	return ;
}

// GETTERS

//ConfigServer::cookies *	ConfigServer::get_cookies( void ) const { return _cookies; }

std::string				ConfigServer::get_host( void ) const { return _host; }

int						ConfigServer::get_port( void ) const { return _port; }

std::string				ConfigServer::get_server_name( void ) const { return _server_name; }

std::string				ConfigServer::get_error_path( void ) const { return _error_path; }

int						ConfigServer::get_body_limit( void ) const { return _body_limit; }

ConfigServer::location	* ConfigServer::get_location( const std::string & virtual_path ) {

	ConfigServer::iterator	it = _locations.begin();
	ConfigServer::iterator	ite = _locations.end();

	ConfigServer::location	* location = NULL;

	while (it != ite) {

		std::string path = virtual_path;
		std::string loc_name = it->get_name();

		if (path[0] != '/')
			path = std::string("/" + path);
		if (path.size() > 1 && path[path.size() - 1] == '/')
			path = path.substr(0, path.size() - 1);

		//if (location != NULL)
		//	std::cout << "it->get_name(): " << it->get_name() << std::endl;
		//std::cout << "loc_name: " << loc_name << std::endl;
		//std::cout << "path: " << path << std::endl;

		if (path.compare(0, loc_name.size(), loc_name) == 0) {
			
			if (location == NULL || location->get_name().size() < loc_name.size())
            	location = &(*it);
        }

		it++;
	}
	//if (location != NULL)
	//	std::cout << "choice: location->get_name(): " << location->get_name() << std::endl;
	//else
	//	std::cout << "choice: NULL" << std::endl;
	return location;
}

ConfigServer::locations	ConfigServer::get_locations( void ) { return _locations; }

std::string	ConfigServer::get_type( const std::string & extension ) const {
	return _types.get_type(extension);
}

// STREAM

std::ostream &	operator<<( std::ostream & o, ConfigServer const & rhs ) {
	o << "host       = " << rhs.get_host() << std::endl;
	o << "port       = " << rhs.get_port() << std::endl;
	o << "serverName = " << rhs.get_server_name() << std::endl;
	o << "errorPath  = " << rhs.get_error_path() << std::endl;
	o << "bodyLimit  = " << rhs.get_body_limit() << std::endl;
	ConfigServer::const_iterator	it = rhs.begin();
	ConfigServer::const_iterator	ite = rhs.end();
	int i = 1;
	while (it != ite) {
		o << BLUE << "location " << i << ":" << DEF << std::endl;
		o << *it;
		it++, i++;
		if (it != ite)
			o << std::endl;
	}
	return o;
}

} // namespace HTTP
