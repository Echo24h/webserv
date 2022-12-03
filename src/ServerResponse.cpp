/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 23:34:37 by gborne            #+#    #+#             */
/*   Updated: 2022/12/03 19:46:27 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ServerResponse.hpp"

ServerResponse::ServerResponse( void ) : _config(NULL) {
	return ;
}

ServerResponse::ServerResponse( ConfigServer * config ) : _config(config) {
	return ;
}

ServerResponse::~ServerResponse() { return ; }

std::string ServerResponse::content( const char * path ) const {

	std::string		str;
	std::ifstream	ifs(path);

	std::string		content;

	while (std::getline(ifs, str))
		content += str += "\r\n";

	return content;
}

std::string	ServerResponse::format( std::vector<std::string> tokens ) const {

	std::string 		content = this->content("www/index.html");
	std::stringstream	ss;
	std::string			html;
	ss << "HTTP/1.1 200 OK\r\n";
	ss << "Content-Type: " << tokens.at(1) << "\r\n";
	ss << "Content-Length: " << content.size() << "\r\n\n";
	ss << content;
	ss >> html;
	return html;
}

std::vector<std::string> tokenize( std::string & recv ) {

	std::vector<std::string> tokens;

	std::stringstream	stream(recv);
	std::string 		cursor;

	while (std::getline(stream, cursor, ' ')) {
		tokens.push_back(cursor);
	}
	return tokens;
}

bool    ServerResponse::_check( std::vector<std::string> tokens ) const {

	if(tokens.size() != 3)
		return false;
	if (tokens.at(2) != "HTTP/1.1")
		return false;

	ConfigServer::locations	loc = _config->getLocations();
	ConfigServer::locations::const_iterator	it = loc.begin();
	ConfigServer::locations::const_iterator	ite = loc.end();

	while (it != ite) {

		std::cout << "<" << it->name << "> <" << tokens.at(1) << ">" << std::endl;

		if (it->name == tokens.at(1)) {

			std::vector<std::string>::const_iterator methods_it = it->methods.begin();
			std::vector<std::string>::const_iterator methods_ite = it->methods.end();

			while (methods_it != methods_ite) {

				std::cout << "<" << *methods_it << "> <" << tokens.at(0) << ">" << std::endl;

				if (*methods_it == tokens.at(0))
					return true;

				methods_it++;
			}
		}
		it++;
	}
	return false;
}

std::string ServerResponse::get( std::string & recv ) const {

	std::vector<std::string> tokens = tokenize(recv);

	if (!_check(tokens))
		return std::string();

	return format(tokens);
}
