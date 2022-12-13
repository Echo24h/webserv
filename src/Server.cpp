/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:27:59 by gborne            #+#    #+#             */
/*   Updated: 2022/12/13 14:37:40 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

namespace HTTP {

Server::Server( void ) : _config(NULL) {
	return;
}

Server::Server( ConfigServer * config ) : _config(config) {
	return ;
}

Server::Server( const Server & src ) {
	*this = src;
	return ;
}

Server::~Server() { return ; }

Server &	Server::operator=( const Server & rhs ) {
	_config = rhs._config;
	return *this;
}

int	Server::_setup_server( void ) const {

	int server_socket;

	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		std::cerr << ERROR << "[Server.cpp] socket() : " << strerror(errno) << std::endl;
		exit(1);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(_config->get_port());
	address.sin_addr.s_addr = inet_addr(_config->get_host().c_str());

	if (bind(server_socket, (struct sockaddr *)&address, sizeof(sockaddr)) == -1) {
		std::cerr << ERROR << "[Server.cpp] bind() : " << strerror(errno) << std::endl;
		exit(1);
	}

	if (listen(server_socket, BACKLOG) == -1) {
		std::cerr << ERROR << "[Server.cpp] listen() : " << strerror(errno) << std::endl;
		exit(1);
	}
	std::cerr << SUCCESS << "Server start at adress " << _config->get_host() << ":" << _config->get_port() << std::endl;

	return server_socket;
}

int	Server::_accept_connection( int server_socket ) const {

	int	client_socket;

	socklen_t addrlen;

	struct sockaddr_in client_addr; /* Adresse du connecté  */

	addrlen = sizeof(client_addr);

	if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addrlen)) == -1)
		std::cerr << ERROR << "[Server.cpp] accept() : " << strerror(errno) << std::endl;
	else
		std::cout << CONNECT << "fd: " << client_socket << ", adress: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

	return client_socket;
}

void	Server::_handle_connexion( int client_socket ) const {

	HTTP::Request	request = Request(_config, client_socket);

	std::cout << RECV << request << std::endl;

	HTTP::Response	response = Response(_config, &request);

	std::string	response_string = response.to_string();

	if (response_string.empty())
		std::cerr << ERROR << "[Server.cpp] Empty response" << std::endl;
	else {
		std::cout << SEND << response << std::endl;

		if (send(client_socket, response_string.c_str(), response_string.size(), 0) == -1)
			std::cerr << ERROR << "[Server.cpp] send() : " << strerror(errno) << std::endl;
	}
	close(client_socket);

	std::cout << DISCONNECT << std::endl;
}

void	Server::run( void ) {

	int server_socket;  /* Ecouter server_socket, nouvelle connection sur client */

	server_socket = _setup_server();

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

					int client_socket = _accept_connection(server_socket);
					FD_SET(client_socket, &current_sockets);
					if (client_socket > max_socket_so_far) {
						max_socket_so_far = client_socket;
					}
				} else {

					// do whatever we do with connecion;
					_handle_connexion(i);
					FD_CLR(i, &current_sockets);
				}
			}
		}
	}
}

} // namespace HTTP
