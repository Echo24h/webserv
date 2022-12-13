/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 19:37:32 by gborne            #+#    #+#             */
/*   Updated: 2022/12/13 17:06:33 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

namespace HTTP {

Request::Request( void ) {
	std::cerr << WARNING << "[Request.cpp] Request() : empty request are construct" << std::endl;
	return ;
}

Request::Request( const ConfigServer * config, const int & client_socket ) : _config(config) {
	std::string buff = _to_string(client_socket);
	std::cout << buff << std::endl;
	_construct(buff);
	return ;
}

Request::Request( const Request & src ) {
	*this = src;
	return ;
}

Request::~Request() {
	return ;
}

Request	& Request::operator=( Request const & rhs ) {
	_method = rhs._method;
	_virtual_path = rhs._virtual_path;
	_real_path = rhs._real_path;
	_cgi = rhs._cgi;
	_query = rhs._query;
	_client_ip = rhs._client_ip;
	_version = rhs._version;
	_ressources = rhs._ressources;
	_config = rhs._config;
	return *this;
}

// GETTERS

std::string	Request::get_method( void ) const {
	return _method;
}

std::string	Request::get_virtual_path( void ) const {
	return _virtual_path;
}

std::string	Request::get_real_path( void ) const {
	return _real_path;
}

std::string	Request::get_cgi( void ) const {
	return _cgi;
}

std::string	Request::get_query( void ) const {
	return _query;
}

std::string	Request::get_client_ip( void ) const {
	return _client_ip;
}

std::string	Request::get_version( void ) const {
	return _version;
}

std::string	Request::get_ressource( const std::string & key ) const {

	Request::ressources::const_iterator it = _ressources.find(key);

	if (it == _ressources.end())
		return std::string();
	return it->second;
}

// FUNCTIONS

std::string	Request::_to_string( const int & client_socket ) const {

	char	buffer[BUFF_SIZE];
	ssize_t	bytes_read = 0;

	bytes_read = recv(client_socket, buffer, BUFF_SIZE, 0);
	if (bytes_read <= 0)
		std::cerr << ERROR << "[Request.cpp] recv() : " << strerror(errno) << std::endl;

	buffer[bytes_read] = '\0';

	return std::string(buffer);
}

std::string	Request::_get_key( const std::string & line ) const {

	std::string	key;
	size_t		n;

	n = line.find(':');
	if (n > 0)
		key = line.substr(0, n);
	return key;
}

std::string	Request::_get_value( const std::string & line ) const {

	std::string	value;
	size_t		n;

	n = line.find(": ");
	if (n > 0)
		value = line.substr(n + 2, line.size() - (n + 2));
	return value;
}

std::string	Request::_get_real_path( const std::string & virtual_path, const std::string & method ) const {

	// secure "../" access
	if (virtual_path.find("..") == (size_t)-1) {

		ConfigServer::location	location = _config->get_location(virtual_path);

		if (location.get_name().empty())
			std::cerr << ERROR << "[Request.cpp] _get_real_path() : location " << virtual_path << " not found" << std::endl;
		else {

			std::string location_name = location.get_name();
			size_t	start = location_name.size();
			if (start > 1)
				start++;

			std::string path = virtual_path.substr(start, virtual_path.size() - start);

			if (location.is_method(method)) {

				std::string location_root = location.get_root();
				std::string locaiton_index = location.get_index();

				if (file_exist(location_root + path))
					return location_root + path;
				else if (location_name.size() == virtual_path.size() && file_exist(location_root + locaiton_index))
					return location_root + locaiton_index;
				else
					std::cerr << ERROR << "[Request.cpp] _get_real_path() : can't found " << location_root + path << std::endl;
				return std::string();
			}
			else
				std::cerr << ERROR << "[Request.cpp] _get_real_path() : can't found method " << method << std::endl;
		}
	}
	return std::string();
}

void	Request::_construct_header( const std::string & line ) {

	std::vector<std::string> tokens = split(line, " ");

	if (tokens.size() != 3)
		std::cerr << ERROR << "[Request.cpp] _construct_header() : wrong format" << std::endl;
	else {
		_method = tokens[0];
		if (_method == "GET" && tokens[1].find('?') != (size_t)-1) {
			_query = tokens[1].substr(tokens[1].find('?') + 1, tokens[1].size() - tokens[1].find('?') - 1);
			_virtual_path = tokens[1].substr(0, tokens[1].find('?'));
			_real_path = _get_real_path(_virtual_path, _method);
		}
		else {
			_virtual_path = tokens[1];
			_real_path = _get_real_path(_virtual_path, _method);
		}
		_version = tokens[2];

		_cgi = _config->get_location(tokens[1]).get_cgi(get_extension(_real_path));
	}
}

void	Request::_construct( const std::string & buff ) {

	std::map<std::string, std::string> tokens;

	std::string			key;
	std::string			value;

	std::vector<std::string> lines = split(buff);

	std::vector<std::string>::iterator	it = lines.begin();
	std::vector<std::string>::iterator	ite = lines.end();

	if (it != ite) {

		// construct header
		_construct_header(*it);
		it++;

		// construct other lines
		while(it != ite) {

			if (!it->empty() && it->find(':') != (size_t)-1) {

				key = _get_key(*it);
				value = _get_value(*it);
				if (key.empty() || value.empty())
					std::cerr << ERROR << "[Request.cpp] _construct() : wrong format" << std::endl;
				else
					_ressources.insert(std::make_pair(key, value));
			}
			else if (_method == "POST") {
				_query = *it;
			}
			it++;
		}
	}
	else
		std::cerr << ERROR << "[Request.cpp] _construct() : empty request" << std::endl;
}

std::ostream &	operator<<( std::ostream & o, Request const & rhs ) {
	o << rhs.get_method() << " ";
	o << rhs.get_virtual_path() << " ";
	o << rhs.get_version();
	return o;
}

} // namespace HTTP
