/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/04 20:21:21 by gborne            #+#    #+#             */
/*   Updated: 2023/06/01 14:43:04 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

namespace HTTP {

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

// Converti l'objet Response en std::string pour être envoyé avec un write()
std::string	Response::to_string( void ) const {

	std::stringstream	ss;
	std::string			response;

	ss << "HTTP/1.1 " << itoa(_code) << "\r\n";
	ss << "Server: " << _config->get_server_name() << "\r\n";
	ss << "Set-Cookie: " << _request->get_ressource("Cookie") << "\r\n";
	if (_request->get_method() != "DELETE") {
		if (_type.empty())
			ss << "Content-Type: " << "text/html; charset=utf-8" << "\r\n";
		else
			ss << "Content-Type: " << _type << "\r\n";
		ss << "Content-Length: " << _content.size() << "\r\n";
		ss << "Connection: keep-alive\r\n\r\n";
		if (!(_code >= 400 && _config->get_error_path().empty()) && !_content.empty() && _code != 204)
			ss << _content << "\r\n";
	}
	response = ss.str();

	return response;
}

// Construit une Response d'erreur
void	Response::_construct_error( const int & code, const std::string & msg ) {
	_code = code;
	if (!(_code >= 400 && _config->get_error_path().empty())) {
		_final_path = _config->get_error_path() + "*";
		_content += read_file(_config->get_error_path() + "header.html");

		_content += "<h1 id=\"error\">Error ";
		_content += itoa(_code);
		_content += "</h1>\n";
		if (!msg.empty()) {
			_content += "<p>";
			_content += msg;
			_content += "</p><br>\n";
		}
		_content += "<p>";
		_content += "<i>WebServ by gborne</i>";
		_content += "</p>\n";
		_content += read_file(_config->get_error_path() + "footer.html");
		_type = _config->get_type("html");
	}
	return ;
}

// Construit une Response de CGI
void	Response::_construct_cgi( void ) {

	CGI	cgi(_config, _request);

	_code = cgi.get_code();

	if (_code < 400) {
		_content = cgi.get_content();
		_type = cgi.get_type();
	}
	else {
		_construct_error(_code, cgi.get_content());
	}
}

// Execute et construit une response Delete
void	Response::_construct_delete( void ) {
	if (remove(_final_path.c_str()) == 0) {
		_code = HTTP::NO_CONTENT;
	}
	else {
		_construct_error(HTTP::INTERNAL_SERVER_ERROR);
	}
}

// Telecharge et construit une response Put
void	Response::_construct_put( void ) {

	if (create_file(_request->get_real_path(), _request->get_content()) == 0)
		_code = 204;
	else
		_code = 404;
}

// Télécharge les fichiers en parsant le multipart/form-data
void Response::_construct_multipartformdata( void ) {

	std::string contentTypeHeader = _request->get_ressource("Content-Type");
	std::string boundary;

	size_t boundaryPos = contentTypeHeader.find("boundary=");
	if (boundaryPos != std::string::npos)
	{
		boundary = contentTypeHeader.substr(boundaryPos + 9);
		// Vérifier si le boundary a été extrait avec succès
		if (!boundary.empty())
		{
			// Séparer les parties du contenu en utilisant le boundary
			std::vector<std::string> parts = split(_request->get_content(), "--" + boundary);

			std::vector<std::string>::iterator it = parts.begin();
			std::vector<std::string>::iterator ite = parts.end();
			
			// Parcourir les parties du contenu
			while (it != ite) {
				
				std::string part = *it;
				// Ignorer les parties vides ou le délimiteur de fin
				if (!part.empty() && part != "--\r\n")
				{
					// Rechercher les informations du fichier
					size_t filenamePos = part.find("filename=\"");
					if (filenamePos != std::string::npos)
					{
						// Extraire le nom du fichier
						size_t filenameStart = filenamePos + 10;
						size_t filenameEnd = part.find("\"", filenameStart);
						std::string filename = part.substr(filenameStart, filenameEnd - filenameStart);

						// Rechercher le contenu du fichier
						size_t fileContentPos = part.find("\r\n\r\n");
						
						if (fileContentPos != std::string::npos) {
							// Extraire le contenu du fichier
							size_t fileContentStart = fileContentPos + 4;
							std::string fileContent = part.substr(fileContentStart);
							if (is_directory(_request->get_real_path()))
								create_file(_request->get_real_path() + filename, fileContent);
							else
								_code = HTTP::NOT_FOUND;
						}
						else {
							std::cerr << WARN << "[Resonse.cpp] _construct_multipartformdata() : Impossible de trouver le contenu du fichier dans la partie multipart." << std::endl;
							_code = HTTP::BAD_REQUEST;
						}
					}
					else {
						std::cerr << WARN << "[Response.cpp] _construct_multipartformdata() : Seul les fichiers peuvent être traiter" << std::endl;
						_code = HTTP::BAD_REQUEST;
						break;
					}
				}
				it++;
			}
		}
		else {
			std::cerr << WARN << "[Response.cpp] _construct_multipartformdata() : Erreur lors de l'extraction du boundary." << std::endl;
			_code = HTTP::BAD_REQUEST;
		}
	}
	else {
		std::cerr << WARN << "[Response.cpp] _construct_multipartformdata() : L'en-tête Content-Type ne contient pas de boundary." << std::endl;
		_code = HTTP::BAD_REQUEST;
	}
	if (_code < 400)
		_code = 204;
}

// Construit la Response a partir de la Request
void	Response::_construct( void ) {

	_final_path = _request->get_real_path();
	_type = _config->get_type(get_extension(_final_path));

	if (_request->get_content().size() > (size_t)_request->get_location()->get_body_limit())
		_construct_error(HTTP::REQUEST_ENTITY_TOO_LARGE, "Request body size exceeds limit");
	else if (_request->get_version() != "HTTP/1.1")
		_construct_error(HTTP::HTTP_VERSION_NOT_SUPPORTED);
	else if (_request->get_method().empty())
		_construct_error(HTTP::METHOD_NOT_ALLOWED);
	else if (_request->get_real_path().empty() && !_request->is_cgi() && _request->get_method() != "POST")
		_construct_error(HTTP::NOT_FOUND);
	else if (_request->get_method() == "DELETE")
		_construct_delete();
	else if ((_request->get_method() == "POST" || _request->get_method() == "PUT") && _request->get_ressource("Content-Type").find("multipart/form-data;") == 0)
		_construct_multipartformdata();
	else if (_request->get_method() == "PUT")
		_construct_put();
	else if (_request->is_cgi())
		_construct_cgi();
	else if (_type.empty())
		_construct_error(HTTP::UNSUPORTED_MEDIA_TYPE);
	else {
		_code = HTTP::OK;
		if (!_final_path.empty())
			_content = read_file(_final_path);
	}
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
