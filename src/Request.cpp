/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 19:37:32 by gborne            #+#    #+#             */
/*   Updated: 2023/06/01 14:46:17 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Request.hpp"

namespace HTTP {

Request::Request( ConfigServer * config, const int & client_socket ) : _config(config) {

	// construit la requette avec des read sur le socket client
	_construct(client_socket);
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

				if (_loc->get_params().empty() && _method != "DELETE") {
					return std::string();
				}
				// POUR DIFFERENCIER "/" et "/*"
				//else if (_loc->get_params() == "/" && ((path.find("/", 1) != (size_t)-1) || path.find("?", 0) != (size_t)-1)) {
				//	return std::string();
				//}
				else if (file_exist(_loc->get_root() + path) || _method == "PUT" ) {
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
				else if (is_directory(_loc->get_root()) && (_method == "PUT" || _method == "POST") && get_ressource("Content-Type").find("multipart/form-data") != (size_t)-1) {
					return remove_double_slashes(_loc->get_root() + "/");
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
			std::cerr << WARN << "[Response.cpp] _construct_put() : Invalid chunk size" << std::endl;
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

			if (bytes == 0)
				break;
			else if (bytes == -1) {
				 std::cerr << WARN << "[Request.cpp] _construct_content() : wrong format fragment request" << std::endl;
				return;
			}

			std::getline(ss, line);

			_content += line.substr(0, bytes);
			content_length += bytes;

		}
		_ressources.insert(std::make_pair("Content-Length", itoa(content_length)));
	}
	// Pour les requettes non fragementées
	else
		_content = buff;

	
}

// Fonction pour lire une requête en mode "chunked" ou avec une taille de contenu
void	Request::_read_request( int socket, std::string & request_data ) {

    char buffer[BUFF_SIZE];

    int bytesRead = 0;
    bool isChunked = false;
	bool isContentLength = false;
    int contentLength = 0;

	int		recv_count = 0;

	//int		delim = -1;

	usleep(REQUEST_INTERVAL);

    // Lire les données jusqu'à ce que la fin de la requête soit atteinte
    while (true) {

		memset(buffer, 0, BUFF_SIZE);
		
        bytesRead = recv(socket, buffer, BUFF_SIZE, 0);

		recv_count++;
		
        if (recv_count > 100000) {
			std::cerr << WARN << "[Request.cpp] _read_request() : timeout reading from socket (read method not specified)" << std::endl;
            break; // Erreur de lecture ou fin de connexion
        }

		if (bytesRead > 0) {
			if (isChunked || isContentLength)
				recv_count = 0;
        	request_data.append(buffer, bytesRead);
		}

        // Vérifier le mode de la requete
        if (!isChunked && !isContentLength) {
            
			size_t headerEndPos = request_data.find("\r\n\r\n");

			if (headerEndPos != std::string::npos) {

                size_t transferEncodingPos = request_data.find("Transfer-Encoding: chunked");
                
				if (transferEncodingPos != std::string::npos) {
                    isChunked = true;
                } 
				else {
                    size_t contentLengthPos = request_data.find("Content-Length:");
                    
					if (contentLengthPos != std::string::npos) {

                        contentLengthPos += sizeof("Content-Length:");

                        size_t contentLengthEndPos = request_data.find("\r\n", contentLengthPos);
                        std::string contentLengthStr = request_data.substr(contentLengthPos, contentLengthEndPos - contentLengthPos);

                        std::istringstream contentLengthStream(contentLengthStr);
                        contentLengthStream >> contentLength;
                        isContentLength = true;
                    }
                }
				if (!isChunked && !isContentLength)
					return ;
            }
        }

        // Vérifier si toutes les données ont été lues
        if (isContentLength && static_cast<int>(request_data.size()) >= contentLength) {
            break;
        }

        // Vérifier si la fin de la requête en mode "chunked" est atteinte
        if (isChunked && request_data.size() >= 7) {
            std::string lastCharacters = request_data.substr(request_data.size() - 7);
            if (lastCharacters == "\r\n0\r\n\r\n") {
                break;
            }
        }
    }
}

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


std::ostream &	operator<<( std::ostream & o, Request const & rhs ) {
	o << rhs.get_method() << " ";
	o << rhs.get_virtual_path() << " ";
	o << rhs.get_version() << " ";
	o << "[" << rhs.get_full_request().size() << "]";
	return o;
}

} // namespace HTTP
