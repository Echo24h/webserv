/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 19:37:32 by gborne            #+#    #+#             */
/*   Updated: 2023/04/21 09:28:19 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

namespace HTTP {

Request::Request( ConfigServer * config, const int & client_socket ) : _config(config) {

	// construit la requette avec des read sur le socket client
	_construct(client_socket);

	// FOR DEBUG
	//create_file("test/last_request", _content);
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

ConfigLocation	* Request::get_location( void ) const {
	return _loc;
}

std::string	Request::get_virtual_path( void ) const {
	return _virtual_path;
}

// Construit le chemin reel et retourne NULL si il n'existe pas
std::string	Request::get_real_path() const {

	// secure "../" access
	if (_virtual_path.find("..") == (size_t)-1) {

		if (_loc != NULL) {

			std::string location_name = _loc->get_name();
			size_t	start = location_name.size();

			std::string path = _virtual_path.substr(start, _virtual_path.size() - start);

			if (!path.empty()) {

				if (_loc->get_params().empty()) {
					return std::string();
				}
				// POUR DIFFERENCIER "/" et "/*"
				//else if (_loc->get_params() == "/" && ((path.find("/", 1) != (size_t)-1) || path.find("?", 0) != (size_t)-1)) {
				//	return std::string();
				//}
				else if (file_exist(_loc->get_root() + path) || _method == "PUT") {
					return remove_double_slashes(_loc->get_root() + path);
				}
				else if (file_exist(_loc->get_root() + path + "/" + _loc->get_index())) {
					return remove_double_slashes(_loc->get_root() + path + "/" + _loc->get_index());
				}
			}
			else {
				if (file_exist(_loc->get_root() + _loc->get_index())) {
					return remove_double_slashes(_loc->get_root() + _loc->get_index());
				}
			}
		}
	}
	return std::string();
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

std::string	Request::get_full_request( void ) const {
	return _full_request;
}

// FUNCTIONS

// Parse la premiere ligne de la requete HTTP
void	Request::_construct_first_line( const std::string & line ) {

	std::vector<std::string> tokens = split(line, " ");

	if (tokens.size() == 3) {

		// Root
		_loc = _config->get_location(tokens[1]);

		// Method
		if (_loc->is_method(tokens[0]))
			_method = tokens[0];

		// Path & Query
		if (_method == "GET" && tokens[1].find('?') != (size_t)-1) {
			_query = tokens[1].substr(tokens[1].find('?') + 1, tokens[1].size() - tokens[1].find('?') - 1);
			_virtual_path = tokens[1].substr(0, tokens[1].find('?'));
			_real_path = get_real_path();
		}
		else {
			_virtual_path = tokens[1];
			_real_path = get_real_path();
		}

		// HTTP Version
		_version = tokens[2];

		// Verifie si CGI ou non
		_cgi = _loc->get_cgi(get_extension(_real_path));
	}
}

// Construit l'object avec le header de la request
void Request::_construct_header( const std::string & buff ) {

	if (buff.size() > 0) {

		std::vector<std::string> lines = split(buff);

		std::vector<std::string>::iterator	it = lines.begin();
		std::vector<std::string>::iterator	ite = lines.end();

		if (it != ite) {

			// construct Method, HTTP version, Path
			_construct_first_line(*it);
			it++;

			// construct ressources
			while(it != ite) {

				if (!it->empty() && it->find(": ") != (size_t)-1) {

					std::string key = get_key(*it, ": ");
					std::string value = get_value(*it, ": ");
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

// Read la requete et construit l'objet Request au fur et a mesure
void Request::_construct( const int & client_socket ) {

	char				buffer[BUFF_SIZE];
	ssize_t				bytes_read;
	std::stringstream	ss;

	bool	is_header = true;

	while((bytes_read = recv(client_socket, buffer, BUFF_SIZE, 0)) > 0) {

		if (bytes_read == (ssize_t)-1) {
			std::cerr << ERROR << "[Request.cpp] _construct() : can't reading from socket" << std::endl;
			break;
		}

		std::string	buff(buffer, bytes_read);

		// Construit le header
		if(is_header) {

			size_t	delim = buff.find("\r\n\r\n");

			//std::cout << "request: " << buff << std::endl; 

			if (delim == (size_t)-1) {
				std::cerr << WARN << "[Request.cpp] _construct() : no delimiteur '\\r\\n\\r\\n' in request" << std::endl;
				_construct_header(buff);
				break;
			}
			_construct_header(buff.substr(0, delim));
			if (delim > 0 && delim < buff.size())
				ss << buff.substr(delim + 4, buff.size() - (delim + 4));
			is_header = false;
		}
		// Ajoute pour le body
		else
			ss << buff;
		usleep(1000);
	}
	if (ss.str().size() > (size_t)_config->get_body_limit())
		std::cerr << ERROR << "[Response.cpp] _read_socket() : body size > body limit" << std::endl;
	else
		_content = ss.str();
	return ;
}

std::ostream &	operator<<( std::ostream & o, Request const & rhs ) {
	o << rhs.get_method() << " ";
	o << rhs.get_virtual_path() << " ";
	o << rhs.get_version();
	return o;
}

} // namespace HTTP
