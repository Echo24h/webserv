/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 20:00:17 by gborne            #+#    #+#             */
/*   Updated: 2022/12/03 03:07:12 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RequestHandler.hpp"

RequestHandler::RequestHandler( void ) : _response(ServerResponse()) {
	return ;
}

RequestHandler::RequestHandler( const ConfigServer & config ) : _response(ServerResponse(config)) {
	return ;
}

std::string	getRequest( int client_socket ) {

	char	buffer[BUFSIZ];
	ssize_t	bytes_read;
	int		msgsize = 0;

	//read the client's message -- the name of the file to read
	while((bytes_read = read(client_socket, buffer+msgsize, sizeof(buffer) - msgsize - 1)) > 0) {
		msgsize += bytes_read;
		if (msgsize > BUFSIZ-1 || buffer[msgsize-1] == '\n')
			break;
	}
	if (bytes_read == -1)
		std::cerr << ERROR << "[Server.cpp] read() : " << strerror(errno) << std::endl;
	buffer[msgsize-1] = 0;

	return std::string(buffer);
}

std::string	getKey( const std::string & line ) {

	std::string	key = "";
	size_t		n;
	
	n = line.find(':');
	if (n > 0)
		key = line.substr(0, n);
	return key;
}

std::string	getValue( const std::string & line ) {

	std::string	value = "";
	size_t		n;
	
	n = line.find(':');
	if (n > 0)
		value = line.substr(n + 1, line.size());
	return value;
}

std::map<std::string, std::string> tokenize( const std::string & request ) {
	
	std::map<std::string, std::string> tokens;

	std::stringstream	stream(request);
	std::string 		cursor;

	std::string			key;
	std::string			value;

	// insert the first line
	getline(stream, cursor, '\n');
	tokens.insert(std::make_pair(std::string("Reqv"), cursor));

	// insert other line
	while(getline(stream, cursor, '\n')) {
		key = getKey(cursor);
		value = getValue(cursor);
		if (key == "" || value == "")
			std::cerr << ERROR << "[Server.cpp] tokenize() : Wrong request format" << std::endl;
		else
			tokens.insert(std::make_pair(key, value));
	}

	return tokens;
}

void	RequestHandler::trait_request( int client_socket ) {

	std::string 						request = getRequest(client_socket);
	std::map<std::string, std::string>	tokens = tokenize(request);

	std::cout << RECV << tokens["Reqv"] << std::endl;

	std::string response = _response.get(tokens["Reqv"]);

	if (response == "")
		std::cerr << ERROR << "Wrong format request" << std::endl;
	else {

		std::cout << SEND << response << std::endl;
		
		if (fork() == 0) { // processus fils

			if (send(client_socket, response.c_str(), response.size(), 0) == -1)
				std::cerr << ERROR << "[Server.cpp] send() : " << strerror(errno) << std::endl;
			
			close(client_socket);
			exit(0);
		}
	}
	close(client_socket);  // Le parent n'a pas besoin de cela

	// Nettoyage des processus fils
	while(waitpid(-1,NULL,WNOHANG) > 0);

	return ;
}
