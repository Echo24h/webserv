/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 19:37:32 by gborne            #+#    #+#             */
/*   Updated: 2023/03/16 01:07:07 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

namespace HTTP {

Request::Request( const ConfigServer * config, const int & client_socket ) : _config(config) {
	std::string buff = _read(client_socket);
	//std::cout << buff << std::endl;
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

bool		Request::is_cgi( void ) const {
	return !_cgi.empty();
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

std::string	Request::get_content( void ) const {
	return _content;
}

// FUNCTIONS

std::string	Request::_read( const int & client_socket ) const {

	char				buffer[BUFF_SIZE];
	ssize_t				bytes_read;
	int					total_bytes = 0;
	std::stringstream	ss;

	while((bytes_read = recv(client_socket, buffer, BUFF_SIZE, 0)) > 0) {
		total_bytes += int(bytes_read);
		if (bytes_read == (ssize_t)-1 || total_bytes >= _config->get_body_limit()) {
			break;
		}
		std::string	string(buffer, bytes_read);
		ss << string;
		// For secure losing data;
		usleep(1000);
	}
	return ss.str();
}

void	Request::_construct_header( const std::string & line ) {

	std::vector<std::string> tokens = split(line, " ");

	if (tokens.size() == 3) {
		_method = tokens[0];
		if (_method == "GET" && tokens[1].find('?') != (size_t)-1) {
			_query = tokens[1].substr(tokens[1].find('?') + 1, tokens[1].size() - tokens[1].find('?') - 1);
			_virtual_path = tokens[1].substr(0, tokens[1].find('?'));
			_real_path = _config->get_real_path(_virtual_path);
		}
		else {
			_virtual_path = tokens[1];
			_real_path = _config->get_real_path(_virtual_path);
		}
		_version = tokens[2];

		_cgi = _config->get_location(tokens[1]).get_cgi(get_extension(_real_path));
	}
}

std::vector<std::string>	split_req( const std::string & buff ) {

	std::vector<std::string>	request;

	size_t	delim = buff.find("\r\n\r\n");

	request.push_back(buff.substr(0, delim));

	request.push_back(buff.substr(delim + 4, buff.size() - (delim + 4)));

	return request;
}

void	Request::_construct( const std::string & buff ) {

	if (buff.empty())
		return ;

	std::map<std::string, std::string> tokens;

	std::string			key;
	std::string			value;

	// separate body and header of request
	std::vector<std::string> request = split_req(buff);

	if (request.size() > 0) {

		if (request.size() > 1)
			_content = request.at(1);

		std::vector<std::string> lines = split(request.at(0));

		std::vector<std::string>::iterator	it = lines.begin();
		std::vector<std::string>::iterator	ite = lines.end();

		if (it != ite) {

			// construct header
			_construct_header(*it);
			it++;

			// construct other lines
			while(it != ite) {

				if (!it->empty() && it->find(": ") != (size_t)-1) {

					key = get_key(*it, ": ");
					value = get_value(*it, ": ");
					if (!key.empty() & !value.empty())
						_ressources.insert(std::make_pair(key, value));
				}
				else if (_method == "POST") {
					_query = *it;
				}
				it++;
			}
		}
	}
}

std::ostream &	operator<<( std::ostream & o, Request const & rhs ) {
	o << rhs.get_method() << " ";
	o << rhs.get_virtual_path() << " ";
	o << rhs.get_version();
	return o;
}

} // namespace HTTP
