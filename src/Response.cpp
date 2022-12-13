/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 20:21:21 by gborne            #+#    #+#             */
/*   Updated: 2022/12/13 15:58:25 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

namespace HTTP {

Response::Response( void ) {
	std::cerr << WARNING << "[Response.cpp] Response() : empty response are construct" << std::endl;
	_construct();
	return ;
}

Response::Response( const ConfigServer * config, const Request * request ) : _config(config), _request(request), _code(HTTP::OK) {
	_construct();
	return ;
}

Response::Response( const Response & src ) {
	*this = src;
	return ;
}

Response::~Response() { return ; }

Response &	Response::operator=( const Response & rhs ) {
	_config = rhs._config;
	_request = rhs._request;
	_code = rhs._code;
	_final_path = rhs._final_path;
	_type = rhs._type;
	_content = rhs._content;
	return *this;
}

// GETTERS

int			Response::get_code( void ) const {
	return _code;
}

std::string	Response::get_final_path( void ) const {
	return _final_path;
}

std::string	Response::get_type( void ) const {
	return _type;
}

// FUNCTIONS

std::string	Response::to_string( void ) const {

	std::stringstream	ss;
	std::string			response;

	ss << "HTTP/1.1 " << itoa(_code) << "\r\n";
	ss << "Content-Type: " << _type << "\r\n";
	ss << "Content-Length: " << _content.size() << "\r\n";
	ss << "Connection: keep-alive\r\n\r\n";
	ss << _content << "\r\n";

	response = ss.str();

	return response;
}

std::string Response::_construct_content( const std::string & path ) const {

	std::string		str;
	std::ifstream	ifs(path.c_str());

	std::string		content;

	if (ifs.fail())
		std::cerr << ERROR << "[Response.cpp] _content() : file " << path << " not found" << std::endl;
	else {
		while (std::getline(ifs, str))
			content += str += '\n';
		ifs.close();
	}
	return content;
}

void	Response::_construct_cgi( void ) {

	_final_path = _request->get_real_path();
	if (_final_path.empty()) {
		std::cerr << ERROR << "[Response.cpp] _construct() : empty path" << std::endl;
		_code = HTTP::NOT_FOUND;
		_final_path = _config->get_error_path() + itoa(_code) + ".html";
		_content = _construct_content(_final_path);
		_type = _config->get_type(get_extension(_final_path));
	}
	else {
		CGI	cgi(_config, _request);
		_code = cgi.get_code();
		_content = cgi.get_content();
		_type = cgi.get_type();
	}
}

void	Response::_construct( void ) {

	std::string	method = _request->get_method();

	if (_request->get_version() != "HTTP/1.1")
		std::cerr << ERROR << "[Response.cpp] _construct() : version \"" << _request->get_version() << "\" isn't supported" << std::endl;
	else if (!_request->get_cgi().empty()) {
		_construct_cgi();
	}
	else if (!method.empty()) {
		_code = HTTP::OK;
		_final_path = _request->get_real_path();
		if (_final_path.empty()) {
			std::cerr << ERROR << "[Response.cpp] _construct() : empty path" << std::endl;
			_code = HTTP::NOT_FOUND;
			_final_path = _config->get_error_path() + itoa(_code) + ".html";
		}
		_type = _config->get_type(get_extension(_final_path));
		if (_type.empty()) {
			std::cerr << ERROR << "[Response.cpp] _construct() : empty type" << std::endl;
			_code = HTTP::NOT_FOUND;
			_final_path = _config->get_error_path() + itoa(_code) + ".html";
		}
		_content = _construct_content(_final_path);
	}
	else
		std::cerr << ERROR << "[Response.cpp] _construct() : Wrong method \"" << method << "\"" << std::endl;
	return ;
}

// STREAM

std::ostream &	operator<<( std::ostream & o, Response const & rhs ) {
	o << itoa(rhs.get_code()) << " ";
	o << rhs.get_final_path() << " ";
	o << rhs.get_type();
	return o;
}

} // namespace HTTP
