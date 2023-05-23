/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 19:37:32 by gborne            #+#    #+#             */
/*   Updated: 2023/05/23 20:57:08 by gborne           ###   ########.fr       */
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
	_full_request = rhs._full_request;
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
		_cgi = _loc->get_cgi(get_extension(tokens[1]));
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
					if (!key.empty() && !value.empty())
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

int get_chunk_size(const std::string& str)
{
    std::stringstream ss(str);
    int result;

	if (is_number(str))
		return atoi(str.c_str());
	else {
		try {
		result = static_cast<int>(strtol(str.c_str(), NULL, 16));
		} catch ( const std::exception & e ) {
			std::cerr << ERROR << "[Response.cpp] _construct_put() : Invalid chunk size" << std::endl;
			return -1;
		}
	}
	return result;
}

// Construit l'object avec le content de la request
void Request::_construct_content( const std::string & buff ) {

	// Pour les requettes fragementées
	if (get_ressource("Content-Length").empty()) {
		
		int content_length = 0;

		std::stringstream ss(buff);

		std::string line;

		while (std::getline(ss, line)) {

			int bytes = get_chunk_size(line.c_str());

			//std::cout << "bytes: " << bytes << std::endl;

			if (bytes == 0)
				break;
			else if (bytes == -1) {
				 std::cerr << ERROR << "[Request.cpp] _construct_content() : wrong format fragment request" << std::endl;
				return;
			}

			std::getline(ss, line);

			_content += line.substr(0, bytes);
			content_length += bytes;

		}
		_ressources.insert(std::make_pair("Content-Length", itoa(content_length)));

		//std::cout << "content_length: " << content_length << std::endl;
		//std::cout << "content_size: " << _content.size() << std::endl;
	}
	// Pour les requettes non fragementées
	else
		_content = buff;

	
}

void	Request::_read_request( int sockfd, std::string & request_data ) {
	
    char buffer[8000];

	//fcntl(sockfd, F_SETFL, O_NONBLOCK);

	int nb_block = 0;

    while (true) {
		
        memset(buffer, 0, 8000);
		
        int bytes_received = recv(sockfd, buffer, 8000, 0);

        if (bytes_received == -1) {
			
			nb_block++;
			
            if ( errno != 11) {
                std::cerr << ERROR << "[Request.cpp] read_request() : can't reading from socket" << std::endl;
                break;
			}
			if (nb_block > 10000) {
				//std::cerr << ERROR << "[Request.cpp] read_request() : timeout reading from socket" << std::endl;
				break;
			}
			
        }
		else {
			nb_block = 0;
			if (bytes_received == 0) {
				// Le socket a été fermé
				break;
			}
			else
				request_data.append(buffer, bytes_received);
		}
	}
}

/*void	Request::_read_request( int sockfd, std::string & request_data ) {
	
    char buffer[BUFF_SIZE];

	fcntl(sockfd, F_SETFL, O_NONBLOCK);

    while (true) {
		
        memset(buffer, 0, BUFF_SIZE);
		
        int bytes_received = recv(sockfd, buffer, BUFF_SIZE, 0);

        if (bytes_received == -1) {
			
            break;
        }
		else {
			if (bytes_received == 0) {
				// Le socket a été fermé
				break;
			}
			else
				request_data.append(buffer, bytes_received);
		}
		usleep(1000);
	}
}*/

// Read la requete du client et construit l'objet Request au fur et a mesure
void Request::_construct( const int & client_socket ) {
	
	_read_request(client_socket, _full_request);

	size_t	delim = _full_request.find("\r\n\r\n");

	if (delim == (size_t)-1) {
		_construct_header(_full_request);
		if (_method == "POST" || _method == "PUT")
			std::cerr << WARN << "[Request.cpp] _construct() : end of head '\\r\\n\\r\\n' not found" << std::endl;
	}
	else {
		_construct_header(_full_request.substr(0, delim));
		_construct_content(_full_request.substr(delim + 4, _full_request.size() - (delim + 4)));
	}
}
/*
void process_request(int sockfd) {
    char buffer[BUFFER_SIZE];
    std::string request_data;

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);

        if (bytes_received < 0) {
            std::cerr << "Error receiving data from socket" << std::endl;
            break;
        }

        if (bytes_received == 0) {
            break;
        }

        request_data.append(buffer, bytes_received);
    }

    std::cout << "Received " << request_data.length() << " bytes: " << std::endl;
    std::cout << request_data << std::endl;
}*/

std::ostream &	operator<<( std::ostream & o, Request const & rhs ) {
	o << rhs.get_method() << " ";
	o << rhs.get_virtual_path() << " ";
	o << rhs.get_version() << " ";
	o << "[" << rhs.get_full_request().size() << "]";
	return o;
}

} // namespace HTTP
