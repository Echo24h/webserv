/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 20:00:17 by gborne            #+#    #+#             */
/*   Updated: 2022/12/02 20:57:29 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/RequestHandler.hpp"

RequestHandler::RequestHandler( void ) {
	return ;
}

char *	getHtml( const char * path ) {
	std::string		html;
	std::string		str;
	std::ifstream	ifs(path);
	char *			header = strdup("HTTP/1.1 200 OK\r\n\r\n");

	while (std::getline(ifs, str))
		html += str;

	char *	result = (char *)malloc(sizeof(char *) * (html.length() + strlen(header) + 1));

	strcpy(result, header);
	strcat(result, html.c_str());

	return result;
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

void	RequestHandler::trait_request( int client_socket ) {

	std::string request = getRequest(client_socket);


	std::cout << RECV << request << std::endl;

	if (fork() == 0) { // processus fils

		char * html = getHtml("www/index.html");

		std::stringstream ss(std::string(html));
		std::string header;
		getline(ss, header, '\n');

		std::cout << SEND << "www/index.html" << std::endl;

		if (send(client_socket, html, strlen(html), 0) == -1)
			std::cerr << ERROR << "[Server.cpp] send() : " << strerror(errno) << std::endl;
		close(client_socket);
		exit(0);
	}
	close(client_socket);  // Le parent n'a pas besoin de cela

	// Nettoyage des processus fils
	while(waitpid(-1,NULL,WNOHANG) > 0);

	return ;
}
