/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 20:00:17 by gborne            #+#    #+#             */
/*   Updated: 2022/12/03 19:47:27 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RequestHandler.hpp"

RequestHandler::RequestHandler( void ) : _config(NULL), _response(ServerResponse()) {
	_response = ServerResponse(_config);
	return ;
}

RequestHandler::RequestHandler( ConfigServer * config ) : _config(config), _response(ServerResponse(config)) {
	return ;
}

RequestHandler::~RequestHandler( void ) { return ; }

size_t split(const std::string &txt, std::vector<std::string> &strs, std::string del)
{
    size_t pos = txt.find( del );
    size_t initialPos = 0;
    strs.clear();

    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + del.size();

        pos = txt.find( del, initialPos );
    }

    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );

    return strs.size();
}

std::string	getRequest( int client_socket ) {

	char	buffer[BUFSIZ];
	ssize_t	bytes_read;

	bytes_read = recv(client_socket, buffer, BUFSIZ, 0);
	if (bytes_read <= 0)
		std::cerr << ERROR << "[Server.cpp] recv() : " << strerror(errno) << std::endl;

	buffer[bytes_read] = '\0';

	return std::string(buffer);
}

std::string	getKey( const std::string & line ) {

	std::string	key;
	size_t		n;

	n = line.find(':');
	if (n > 0)
		key = line.substr(0, n);
	return key;
}

std::string	getValue( const std::string & line ) {

	std::string	value;
	size_t		n;

	n = line.find(": ");
	if (n > 0)
		value = line.substr(n + 2, line.size() - (n + 2));
	return value;
}

std::map<std::string, std::string> tokenize( const std::string & request ) {

	std::map<std::string, std::string> tokens;

	std::stringstream	stream(request);
	std::string 		cursor;

	std::string			key;
	std::string			value;

	std::vector<std::string> lines;

	split(request, lines, "\r\n");

	std::vector<std::string>::iterator	it = lines.begin();
	std::vector<std::string>::iterator	ite = lines.end();

	if (it != ite) {

		// insert the first line
		tokens.insert(std::make_pair(std::string("Reqv"), *it));
		it++;

		// insert other line
		while(it != ite) {

			if (!it->empty()) {

				key = getKey(*it);
				value = getValue(*it);
				if (key.empty() || value.empty())
					std::cerr << ERROR << "[Server.cpp] tokenize() : Wrong request format" << std::endl;
				else
					tokens.insert(std::make_pair(key, value));
			}
			it++;
		}

	}

	return tokens;
}

void	RequestHandler::trait_request( int client_socket ) {

	std::string 						request = getRequest(client_socket);
	std::map<std::string, std::string>	tokens = tokenize(request);

	std::cout << RECV << tokens["Reqv"] << std::endl;

	std::string response = _response.get(tokens["Reqv"]);

	if (response.empty())
		std::cerr << ERROR << "Wrong format request" << std::endl;
	else {
		std::cout << SEND << response << std::endl;

		// FORK ?
		if (send(client_socket, response.c_str(), response.size(), 0) == -1)
			std::cerr << ERROR << "[Server.cpp] send() : " << strerror(errno) << std::endl;
	}
	close(client_socket);  // Le parent n'a pas besoin de cela

	// Nettoyage des processus fils
	while(waitpid(-1,NULL,WNOHANG) > 0);

	return ;
}
