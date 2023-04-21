/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:27:59 by gborne            #+#    #+#             */
/*   Updated: 2023/04/22 00:20:54 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

namespace HTTP {

Server::Server( Config * config ) : _config(config) {
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

void	Server::_setup_server( void ) {

	Config::iterator	it = _config->begin();
	Config::iterator	ite = _config->end();

	while (it != ite) {

		int server_socket;

		struct sockaddr_in address;
		memset(&address, 0, sizeof(address));

		if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
			throw std::runtime_error("[Server.cpp] socket() : " + std::string(strerror(errno)));

		address.sin_family = AF_INET;
		address.sin_port = htons(it->get_port());
		address.sin_addr.s_addr = inet_addr(it->get_host().c_str());

		if (bind(server_socket, (struct sockaddr *)&address, sizeof(sockaddr)) == -1)
			throw std::runtime_error("[Server.cpp] bind() : " + std::string(strerror(errno)));

		if (listen(server_socket, BACKLOG) == -1)
			throw std::runtime_error("[Server.cpp] listen() : " + std::string(strerror(errno)));

		std::cout << SUCCESS << "Listening enabled on address [" << it->get_host() << ":" << it->get_port() << "]" << std::endl;

		_listens.insert(std::make_pair(server_socket, &(*it)));

		it++;
	}
	if (_listens.size() > 0)
		std::cout << SUCCESS << "Server start" << std::endl;
	else
		throw std::runtime_error("[Server.cpp] _setup_server() : no socket listening");

}

int	Server::_accept_connection( const int & server_socket ) const {

	int	client_socket;

	socklen_t addrlen;

	struct sockaddr_in client_addr; /* Adresse du connecté  */

	addrlen = sizeof(client_addr);

	if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addrlen)) == -1)
		std::cerr << ERROR << "[Server.cpp] accept() : " << strerror(errno) << std::endl;

	fcntl(client_socket, F_SETFL, O_NONBLOCK);

	return client_socket;
}

void	Server::_handle_connexion( const int & client_socket, ConfigServer * config ) const {

	// On récupère les informations du client pour l'affichage dans la console
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    getsockname(client_socket, (struct sockaddr *)&addr, &addrlen);

	std::cout << YELLOW << "New connection" << DEF << std::endl;

	HTTP::Request	request = Request(config, client_socket);

	std::cout << RECV << inet_ntoa(addr.sin_addr) << " : " <<  request << std::endl;

	HTTP::Response	response = Response(config, &request);

	std::string	response_string = response.to_string();

	if (response_string.empty())
		std::cerr << ERROR << "[Server.cpp] Empty response" << std::endl;
	else {
		std::cout << SEND << inet_ntoa(addr.sin_addr) << " : " << response << std::endl;

		if (send(client_socket, response_string.c_str(), response_string.size(), 0) == -1)
			std::cerr << ERROR << "[Server.cpp] send() : " << strerror(errno) << std::endl;
	}
	close(client_socket);
}

void	Server::run( void ) {

	int									max_socket_so_far = 0;

	fd_set								current_sockets, ready_sockets;

	std::map<int, ConfigServer *>	clients_config;

	_setup_server();

	// initialize my current set
	FD_ZERO(&current_sockets);

	Server::listens::iterator	it = _listens.begin();
	Server::listens::iterator	ite = _listens.end();

	while (it != ite) {
		int server_socket = it->first;
		FD_SET(server_socket, &current_sockets);
		if (server_socket > max_socket_so_far)
			max_socket_so_far = server_socket;
		it++;
	}

	while(1) {

		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 10000; // 10 ms

		//because select is destructor
		ready_sockets = current_sockets;

		if (select(max_socket_so_far + 1, &ready_sockets, NULL, NULL, &timeout) < 0)
			throw std::runtime_error("[Server.cpp] select() : " + std::string(strerror(errno)));

		for (int i = 0; i <= max_socket_so_far; i++) {
			if (FD_ISSET(i, &ready_sockets)) {

				Server::listens::iterator it = _listens.find(i);

				if (it != _listens.end()) {
					//this is a new connection
					int client_socket = _accept_connection(it->first);
					clients_config.insert(std::make_pair(client_socket, it->second));
					FD_SET(client_socket, &current_sockets);
					if (client_socket > max_socket_so_far) {
						max_socket_so_far = client_socket;
					}
				} else {
					FD_CLR(i, &current_sockets);
					_handle_connexion(i, clients_config.find(i)->second);
					clients_config.erase(i);
				}
			}
		}
	}
}

} // namespace HTTP
