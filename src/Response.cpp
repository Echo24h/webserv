/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 20:21:21 by gborne            #+#    #+#             */
/*   Updated: 2022/12/08 15:01:36 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

namespace HTTP {

Response::Response( void ) : _config(NULL) {
	return ;
}

Response::Response( ConfigServer * config, const Request & request ) : _config(config) {
	_serialize(request);
	return ;
}

Response::Response( const Response & src ) {
	*this = src;
	return ;
}

Response::~Response() { return ; }

Response &	Response::operator=( const Response & rhs ) {
	_file_path = rhs._file_path;
	_file_type = rhs._file_type;
	_config = rhs._config;
	return *this;
}

std::string	Response::to_string( void ) const {

	std::stringstream	ss;
	std::string			response;

	if (_code == HTTP::OK) {

		std::string 		content = _content(_file_path);

		ss << "HTTP/1.1 " << HTTP::OK << " OK\r\n";
		ss << "Content-Type: " << _file_type << "\r\n";
		ss << "Content-Length: " << content.size() << "\r\n";
		ss << "Connection: keep-alive\r\n\r\n";
		ss << content;
	}
	else if (_code == HTTP::NOT_FOUND) {

		std::string 	content = _content(_config->getErrorPath() + "404.html");

		ss << "HTTP/1.1 " << _code << " Not Found\r\n";
		ss << "Content-Type: " << "text/html" << "\r\n";
		ss << "Content-Length: " << content.size() << "\r\n";
		ss << "Connection: keep-alive\r\n\r\n";
		ss << content;
	}
	response = ss.str();

	return response;
}

std::string	Response::file_path( void ) const {
	return _file_path;
}

std::string	Response::file_type( void ) const {
	return _file_type;
}

int	Response::code( void ) const {
	return _code;
}

void	Response::_serialize( const Request & request ) {

	if (request.version() != "HTTP/1.1")
		std::cerr << ERROR << "[Response.cpp] _serialize() : Wrong request version" << std::endl;
	else if (request.method() == "GET")
		_get(request.location());

	if (_file_path.empty() || _file_type.empty())
		_code = HTTP::NOT_FOUND;
	else
		_code = HTTP::OK;
}

std::string Response::_content( const std::string & path ) const {

	std::string		str;
	std::ifstream	ifs(path.c_str());

	std::string		content;

	while (std::getline(ifs, str))
		content += str += '\n';

	ifs.close();

	return content;
}

std::string	Response::_get_file_path( const std::string & location, const std::string & method ) const {

	// secure "../" access
	if (location.find("..") == (size_t)-1) {

		// check location in config
		ConfigServer::locations				loc = _config->getLocations();
		ConfigServer::locations::iterator	it = loc.begin();
		ConfigServer::locations::iterator	ite = loc.end();

		Location *							target_loc = NULL;

		while (it != ite) {
			if (location.find(it->name) == 0)
				if (target_loc == NULL || target_loc->name.size() < it->name.size())
					target_loc = &(*it);
			it++;
		}

		if (target_loc == NULL)
			std::cerr << ERROR << "[Response.cpp] _get_file_path() : location " << location << " not found" << std::endl;
		else {

			std::vector<std::string>::const_iterator methods_it = target_loc->methods.begin();
			std::vector<std::string>::const_iterator methods_ite = target_loc->methods.end();

			size_t	start;
			target_loc->name.size() > 1 ? start = target_loc->name.size() + 1 : start = target_loc->name.size();
			std::string path = location.substr(start, location.size() - start);

			while (methods_it != methods_ite) {

				if (*methods_it == method) {
					if (file_exist(target_loc->root + path))
						return target_loc->root + path;
					else if (target_loc->name.size() == location.size() && file_exist(target_loc->root + target_loc->index))
						return target_loc->root + target_loc->index;
					else
						std::cerr << ERROR << "[Response.cpp] _get_file_path() : file " << target_loc->root + path << " or " << target_loc->root + target_loc->index << " not found" << std::endl;
					return std::string();
				}
				methods_it++;
			}
			std::cerr << ERROR << "[Response.cpp] _get_file_path() : method " << method << " not found" << std::endl;
		}
	}
	return std::string();
}

std::string	Response::_get_file_type( const std::string & file_path ) const {

	if (!file_path.empty()) {

		std::string	file = *split(file_path, "/").rbegin();

		if (!file.empty()) {

			std::string	extension = *split(file, ".").rbegin();

			if (extension == "html")
				return	"text/html";
			else if (extension == "css")
				return "text/css";
			else if (extension == "ico")
				return "image/x-icon";
			else if (extension == "png")
				return "image/png";
		}
	}
	return std::string();
}

void	Response::_get( const std::string & location ) {

	_file_path = _get_file_path(location, "GET");
	if (_file_path.empty())
		std::cerr << ERROR << "[Response.cpp] _get() : " + location + " doesn't exist" << std::endl;
	_file_type = _get_file_type(_file_path);
	if (!_file_path.empty() && _file_type.empty())
		std::cerr << ERROR << "[Response.cpp] _get() : extension file doesn't exist" << std::endl;
}

std::ostream &	operator<<( std::ostream & o, Response const & rhs ) {
	o << rhs.code() << " ";
	o << rhs.file_path() << " ";
	o << rhs.file_type();
	return o;
}

} // namespace HTTP
