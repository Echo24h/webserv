/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:27:59 by gborne            #+#    #+#             */
/*   Updated: 2022/12/02 20:50:49 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// http://vidalc.chez.com/lf/socket.html

#include "../inc/Server.hpp"

Server::Server( void ) { return ; }

int	setup_server() {

	int server_socket;

	struct sockaddr_in my_addr;    /* Adresse */

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << ERROR << "[Server.cpp] socket() : " << strerror(errno) << std::endl;
		exit(1);
	}

	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-remplissage avec mon IP */
	bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */

	if (bind(server_socket, (struct sockaddr *)&my_addr, sizeof(sockaddr)) == -1) {
		std::cerr << ERROR << "[Server.cpp] bind() : " << strerror(errno) << std::endl;
		exit(1);
	}

	if (listen(server_socket, BACKLOG) == -1) {
		std::cerr << ERROR << "[Server.cpp] listen() : " << strerror(errno) << std::endl;
		exit(1);
	}
	return server_socket;
}

int	accept_connection( int server_socket ) {

	int	client_socket;

	socklen_t addrlen;

	struct sockaddr_in client_addr; /* Adresse du connecté  */

	addrlen = sizeof(client_addr);

	if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addrlen)) == -1)
		std::cerr << ERROR << "[Server.cpp] accept() : " << strerror(errno) << std::endl;
	else
		std::cout << REQUEST << inet_ntoa(client_addr.sin_addr) << std::endl;

	return client_socket;
}

void	Server::run( void ) {

	int server_socket;  /* Ecouter server_socket, nouvelle connection sur client */

	server_socket = setup_server();

	int	max_socket_so_far = 0;

	fd_set	current_sockets, ready_sockets;

	// initialize my current set
	FD_ZERO(&current_sockets);
	FD_SET(server_socket, &current_sockets);
	max_socket_so_far = server_socket;

	while(1) {  /* main accept() loop */

		//because select is destructor
		ready_sockets = current_sockets;

		if (select(max_socket_so_far + 1, &ready_sockets, NULL, NULL, NULL) < 0) {
			std::cerr << ERROR << "[Server.cpp] select() : " << strerror(errno) << std::endl;
			exit(0);
		}

		for (int i = 0; i <= max_socket_so_far; i++) {
			if (FD_ISSET(i, &ready_sockets)) {
				if (i == server_socket) {
					//this is a new connection

					int client_socket = accept_connection(server_socket);
					FD_SET(client_socket, &current_sockets);
					if (client_socket > max_socket_so_far) {
						max_socket_so_far = client_socket;
					}
				} else {

					// do whatever we do with connecion;
					RequestHandler::trait_request(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
}
