/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 19:37:32 by gborne            #+#    #+#             */
/*   Updated: 2022/12/05 18:34:16 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

namespace HTTP {

Request::Request( void ) {
	return ;
}

Request::Request( const std::string & buff ) {
	_serialize(buff);
	return ;
}

Request::Request( int client_socket ) {
	std::string buff = to_string(client_socket);
	_serialize(buff);
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
	_version = rhs._version;
	_method = rhs._method;
	_location = rhs._location;
	_ressource = rhs._ressource;
	return *this;
}

std::string	Request::to_string( int client_socket ) const {

	char	buffer[BUFF_SIZE];
	ssize_t	bytes_read;

	bytes_read = recv(client_socket, buffer, BUFF_SIZE, 0);
	if (bytes_read <= 0)
		std::cerr << ERROR << "[Request.cpp] recv() : " << strerror(errno) << std::endl;

	buffer[bytes_read] = '\0';

	return std::string(buffer);
}

// GETTERS

std::string	Request::version( void ) const {
	return _version;
}

std::string	Request::method( void ) const {
	return _method;
}

std::string	Request::location( void ) const {
	return _location;
}

std::map<std::string, std::string>	Request::ressource( void ) const {
	return _ressource;
}

void	Request::_header( const std::string & line ) {

	std::vector<std::string> tokens = split(line, " ");

	if (tokens.size() != 3)
		std::cerr << ERROR << "[Request.cpp] _header() : Wrong format" << std::endl;
	else {
		_method = tokens[0];
		_location = tokens[1];
		_version = tokens[2];
	}
}

void	Request::_serialize( const std::string & buff ) {

	std::map<std::string, std::string> tokens;

	std::string			key;
	std::string			value;

	std::vector<std::string> lines = HTTP::split(buff);

	std::vector<std::string>::iterator	it = lines.begin();
	std::vector<std::string>::iterator	ite = lines.end();

	if (it != ite) {

		// serialize header
		_header(*it);
		it++;

		// serialize other line
		while(it != ite) {

			if (!it->empty()) {

				key = HTTP::get_key(*it);
				value = HTTP::get_value(*it);
				if (key.empty() || value.empty())
					std::cerr << ERROR << "[Request.cpp] _serialize() : Wrong format" << std::endl;
				else
					_ressource.insert(std::make_pair(key, value));
			}
			it++;
		}

	}
	else
		std::cerr << ERROR << "[Request.cpp] _serialize() : Empty request" << std::endl;
}

std::ostream &	operator<<( std::ostream & o, Request const & rhs ) {
	o << rhs.method() << " ";
	o << rhs.location() << " ";
	o << rhs.version();
	return o;
}

} // namespace HTTP
