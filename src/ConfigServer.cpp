/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 16:20:48 by gborne            #+#    #+#             */
/*   Updated: 2023/02/11 17:02:37 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ConfigServer.hpp"

namespace HTTP {

ConfigServer::ConfigServer( void ) : _host("localhost"), _port(80), _error_path("err/"), _body_limit(1980) {
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
		throw std::invalid_argument(std::string("[ConfigServer.cpp] host is empty").c_str());
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
	if (error_path.empty())
		throw std::invalid_argument("[ConfigServer.cpp] error path is empty");
	_error_path = error_path;
	return ;
}

void	ConfigServer::set_body_limit( const std::string & body_limit ) {

	char		extension = *body_limit.rbegin();
	std::string	num = body_limit.substr(0, body_limit.size() - 1);

	if (num.empty() || !is_number(num))
		throw std::invalid_argument("[ConfigServer.cpp] wrong body_limit");

	if (extension == 'M')
		_body_limit = atoi(num.c_str()) * 1000000;
	else if (extension == 'K')
		_body_limit = atoi(num.c_str()) * 1000;
	else
		throw std::invalid_argument("[ConfigServer.cpp] inknow body_limit type");

	if (_body_limit < 2)
		throw std::invalid_argument("[ConfigServer.cpp] body must be larger than 1Ko");
	//std::cout << _body_limit << std::endl;
	return ;
}

void	ConfigServer::new_location( const ConfigServer::location & location ) {
	_locations.push_back(location);
	return ;
}

// GETTERS

std::string				ConfigServer::get_host( void ) const { return _host; }

int						ConfigServer::get_port( void ) const { return _port; }

std::string				ConfigServer::get_server_name( void ) const { return _server_name; }

std::string				ConfigServer::get_error_path( void ) const { return _error_path; }

int						ConfigServer::get_body_limit( void ) const { return _body_limit; }

ConfigServer::location	ConfigServer::get_location( const std::string & virtual_path ) const {

	ConfigServer::const_iterator	it = _locations.begin();
	ConfigServer::const_iterator	ite = _locations.end();

	ConfigServer::location	location;

	while (it != ite) {
		std::string location_name = it->get_name();
		if (virtual_path.find(location_name) == 0)
			if (location.get_name().empty() || location_name.size() < location_name.size())
				location = *it;
		it++;
	}
	return location;
}

ConfigServer::locations	ConfigServer::get_locations( void ) const { return _locations; }

std::string	ConfigServer::get_type( const std::string & extension ) const {
	return _types.get_type(extension);
}

std::string	ConfigServer::get_real_path( const std::string & virtual_path ) const {

	// secure "../" access
	if (virtual_path.find("..") == (size_t)-1) {

		ConfigServer::location	location = get_location(virtual_path);

		if (!location.get_name().empty()) {

			std::string location_name = location.get_name();
			size_t	start = location_name.size();
			if (start > 1)
				start++;

			std::string path = virtual_path.substr(start, virtual_path.size() - start);

			std::string location_root = location.get_root();
			std::string locaiton_index = location.get_index();

			if (file_exist(location_root + path))
				return location_root + path;
			else if (location_name.size() == virtual_path.size() && file_exist(location_root + locaiton_index))
				return location_root + locaiton_index;
			return std::string();
		}
	}
	return std::string();
}

// STREAM

std::ostream &	operator<<( std::ostream & o, ConfigServer const & rhs ) {
	o << "host       = " << rhs.get_host() << std::endl;
	o << "port       = " << rhs.get_port() << std::endl;
	o << "serverName = " << rhs.get_server_name() << std::endl;
	o << "errorPath  = " << rhs.get_error_path() << std::endl;
	o << "bodyLimit  = " << rhs.get_body_limit() << std::endl;
	ConfigServer::locations	loc = rhs.get_locations();
	ConfigServer::iterator	it = loc.begin();
	ConfigServer::iterator	ite = loc.end();
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
