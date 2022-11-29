/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 17:24:04 by gborne            #+#    #+#             */
/*   Updated: 2022/11/29 15:50:17 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Config.hpp"

Config::Config( void ) : _path("../conf/default.conf"), _servers(servers()) {
	_initServers(_tokenizeConfig());
	return ;
}

Config::Config( const char * path ) : _path(path), _servers(servers()) {
	_initServers(_tokenizeConfig());
	return ;
}

Config::Config( Config const & src ) {
	*this = src;
	return ;
}

Config::~Config( void ) {
	return ;
}

Config & Config::operator=( Config const & rhs ) {
	_path = rhs._path;
	_servers = rhs._servers;
	return *this;
}

Config::iterator Config::begin( void ) {
	return _servers.begin();
}

Config::iterator Config::end( void ) {
	return _servers.end();
}

Config::const_iterator Config::begin( void ) const {
	return _servers.begin();
}

Config::const_iterator Config::end( void ) const {
	return _servers.end();
}

Config::tokens Config::_tokenizeConfig( void ) const {

	Config::tokens	tokens;

	std::ifstream	ifs(_path);
	std::string		str;

	if (ifs.fail())
		throw std::invalid_argument("[Config.cpp] \"" + std::string(_path) + "\" this config doens't exist");

	while (std::getline(ifs, str)) {

		size_t	pos = 0;
		size_t	len = 0;

		while (str[pos]) {

			if (str[pos] == '{' || str[pos] == '}') {
				tokens.push_back(std::string(1, str[pos]));
			}
			else if (str[pos] == ' ' || str[pos] == '\t' || !str[pos + 1]) {
				if (len > 0) {
					if (!str[pos + 1])
						tokens.push_back(str.substr(pos - len, len + 1));
					else
						tokens.push_back(str.substr(pos - len, len));
				}
				len = 0;
			}

			if (str[pos] == ' ' || str[pos] == '\t')
				pos++;
			else
				pos++, len++;
		}
	}

	ifs.close();

	return tokens;
}

Config::tokens::iterator Config::_traitServLoc( Server * server, Config::tokens::iterator it, Config::tokens::iterator ite ) {

	std::string					name;
	std::vector<std::string>	methods;
	std::string					index;
	std::string					root;
	std::string					php;
	std::string					cgi;

	if (*(*(++it)).begin() != '/')
		throw std::invalid_argument("[Config.cpp] expected '/' at 'location' in file \"" + std::string(_path) + "\"");
	name = *it;
	it++;
	if (*it != "{")
		throw std::invalid_argument("[Config.cpp] expected '{' in file \"" + std::string(_path) + "\"");
	it++;
	while (*it != "}" && it != ite) {

		if (*it == "methods") {
			it++;
			while (*(*it).rbegin() == ',') {
				methods.push_back((*it).substr(0, (*it).length() - 1));
				it++;
			}
			if (*(*it).rbegin() != ';')
				throw std::invalid_argument("[Config.cpp] expected ';' at 'methods' in file \"" + std::string(_path) + "\"");
			methods.push_back((*it).substr(0, (*it).length() - 1));
		}
		else if (*it == "index") {
			it++;
			if (*(*it).rbegin() != ';')
				throw std::invalid_argument("[Config.cpp] expected ';' at 'index' in file \"" + std::string(_path) + "\"");
			index = (*it).substr(0, (*it).length() - 1);
		}
		else if (*it == "root") {
			it++;
			if (*(*it).rbegin() != ';')
				throw std::invalid_argument("[Config.cpp] expected ';' at 'root' in file \"" + std::string(_path) + "\"");
			root = (*it).substr(0, (*it).length() - 1);
		}
		else if (*it == "php") {
			it++;
			if (*(*it).rbegin() != ';')
				throw std::invalid_argument("[Config.cpp] expected ';' at 'php' in file \"" + std::string(_path) + "\"");
			php = (*it).substr(0, (*it).length() - 1);
		}
		else if (*it == "cgi") {
			it++;
			if (*(*it).rbegin() != ';')
				throw std::invalid_argument("[Config.cpp] expected ';' at 'cgi' in file \"" + std::string(_path) + "\"");
			cgi = (*it).substr(0, (*it).length() - 1);
		}
		else
			throw std::invalid_argument("[Config.cpp] unknown argument '" + *it + "' in file \"" + std::string(_path) + "\"");
		it++;
	}
	if (*it != "}")
		throw std::invalid_argument("[Config.cpp] expected '}' in file \"" + std::string(_path) + "\"");
	server->addLocation(name, methods, index, root, php, cgi);
	return it;
}

Config::tokens::iterator Config::_traitServParam( Server * server, Config::tokens::iterator it ) {

	if (*it == "host") {
		it++;
		if (*(*it).rbegin() != ';')
			throw std::invalid_argument("[Config.cpp] expected ';' at 'host' in file \"" + std::string(_path) + "\"");
		server->setHost((*it).substr(0, (*it).length() - 1));
	}
	else if (*it == "port") {
		it++;
		if (*(*it).rbegin() != ';')
			throw std::invalid_argument("[Config.cpp] expected ';' at 'port' in file \"" + std::string(_path) + "\"");
		server->setPort(atoi((*it).substr(0, (*it).length() - 1).c_str()));
	}
	else if (*it == "server_name") {
		it++;
		if (*(*it).rbegin() != ';')
			throw std::invalid_argument("[Config.cpp] expected ';' at 'server_name' in file \"" + std::string(_path) + "\"");
		server->setServerName((*it).substr(0, (*it).length() - 1));
	}
	else if (*it == "error") {
		it++;
		if (*(*it).rbegin() != ';')
			throw std::invalid_argument("[Config.cpp] expected ';' at 'error' in file \"" + std::string(_path) + "\"");
		server->setErrorPath((*it).substr(0, (*it).length() - 1));
	}
	else if (*it == "body_limit") {
		it++;
		if (*(*it).rbegin() != ';')
			throw std::invalid_argument("[Config.cpp] expected ';' at 'body_limit' in file \"" + std::string(_path) + "\"");
		server->setBodyLimit(atoi((*it).substr(0, (*it).length() - 1).c_str()));
	}
	else
		throw std::invalid_argument("[Config.cpp] unknown argument '" + *it + "' in file \"" + std::string(_path) + "\"");
	return it;
}

void Config::_initServers( Config::tokens tokens ) {

	Config::tokens::iterator	it = tokens.begin();
	Config::tokens::iterator	ite = tokens.end();

	while (it != ite) {
		//std::cout << "<" << *(it++) << ">" << std::endl;

		if (*it == "server") {

			Server server;

			it++;
			if (*it != "{")
				throw std::invalid_argument("[Config.cpp] expected '{' in file \"" + std::string(_path) + "\"");
			it++;
			while (it != ite && *it != "}" && *it != "server") {
				if (*it == "location")
					it = _traitServLoc(&server, it, ite);
				else
					it = _traitServParam(&server, it);
				if (it != ite)
					it++;
			}
			if (*it != "}")
				throw std::invalid_argument("[Config.cpp] expected '}' in file \"" + std::string(_path) + "\"");

			_servers.push_back(server);
		}
		it++;
	}

	return ;
}

// STREAM

std::ostream &	operator<<( std::ostream & o, Config const & rhs ) {
	Config::const_iterator	it = rhs.begin();
	Config::const_iterator	ite = rhs.end();
	int i = 1;
	while (it != ite) {
		std::cout << "\033[1;33m" << "Server " << i << "\033[0m" << std::endl;
		std::cout << *it << std::endl;
		it++;
		i++;
	}
	return o;
}
