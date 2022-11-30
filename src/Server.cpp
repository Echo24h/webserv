/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:27:59 by gborne            #+#    #+#             */
/*   Updated: 2022/11/30 20:41:36 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// http://vidalc.chez.com/lf/socket.html

#include "../inc/Server.hpp"

Server::Server( void ) { return ; }

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

void	Server::run( void ) {

	int sockfd, new_fd;  /* Ecouter sock_fd, nouvelle connection sur new_fd */
	socklen_t addrlen;
	struct sockaddr_in my_addr;    /* Adresse */
	struct sockaddr_in their_addr; /* Adresse du connecté  */

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << "[Server.cpp] socket() : " << strerror(errno) << std::endl;
		exit(1);
	}

	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-remplissage avec mon IP */
	bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(sockaddr)) == -1) {
		std::cerr << "[Server.cpp] bind() : " << strerror(errno) << std::endl;
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		std::cerr << "[Server.cpp] listen() : " << strerror(errno) << std::endl;
		exit(1);
	}

	while(1) {  /* main accept() loop */

		addrlen = sizeof(their_addr);

		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addrlen)) == -1) {
			std::cerr << "[Server.cpp] accept() : " << strerror(errno) << std::endl;
			continue;
		}
		std::cout << "Serveur: Reçu connection de " << inet_ntoa(their_addr.sin_addr) << std::endl;
		if (fork() == 0) { /* processus fils */

			char * html = getHtml("www/index.html");

			if (send(new_fd, html, strlen(html), 0) == -1)
				std::cerr << "[Server.cpp] send() : " << strerror(errno) << std::endl;
			close(new_fd);
			exit(0);
		}
		close(new_fd);  /* Le parent n'a pas besoin de cela */

		while(waitpid(-1,NULL,WNOHANG) > 0); /* Nettoyage des processus fils */
	}
}
