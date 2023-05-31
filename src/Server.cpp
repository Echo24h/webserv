/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:27:59 by gborne            #+#    #+#             */
/*   Updated: 2023/05/31 17:57:29 by gborne           ###   ########.fr       */
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

// Configure les ports et demarre l'ecoute
void	Server::_setup_server( void ) {

	Config::iterator	it = _config->begin();
	Config::iterator	ite = _config->end();

	while (it != ite) {

		int server_socket;
		int on = 1;

		struct sockaddr_in address;
		memset(&address, 0, sizeof(address));

		if ((server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
			throw std::runtime_error("[Server.cpp] socket() : " + std::string(strerror(errno)));

		//fcntl(server_socket, F_SETFL, O_NONBLOCK);

		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
			throw std::runtime_error("[Server.cpp] setsockopt() : " + std::string(strerror(errno)));

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

// Accept une connection client et configure le Socket
int	Server::_accept_connection( const int & server_socket ) const {

	int	client_socket;

	socklen_t addrlen;

	struct sockaddr_in client_addr; /* Adresse du connecté  */

	addrlen = sizeof(client_addr);

	if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addrlen)) == -1)
		std::cerr << ERROR << "[Server.cpp] accept() : " << strerror(errno) << std::endl;

	//fcntl(client_socket, F_SETFL, O_NONBLOCK);

	// Configuration de l'option SO_LINGER
    /*struct linger so_linger;
    so_linger.l_onoff = 1;    // Activer SO_LINGER
    so_linger.l_linger = 5;  // Temps d'attente en secondes

    if (setsockopt(client_socket, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger)) == -1) {
        std::cerr << ERROR << "[Server.cpp] setsockopt() : " << strerror(errno) << std::endl;
        close(client_socket);
        return -1;
    }*/


	return client_socket;
}

static int count = 0;

// Gère une connexion (recv() Request + send() Response)
void	Server::_handle_connexion( const int & client_socket, ConfigServer * config ) const {

	// On récupère les informations du client pour l'affichage dans la console
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    getsockname(client_socket, (struct sockaddr *)&addr, &addrlen);

	// L'objet Request lit la requette client et creer un objet
	HTTP::Request	request = Request(config, client_socket);

	if (config->get_logs() == "short" || config->get_logs() == "full")
		std::cout << RECV << inet_ntoa(addr.sin_addr) << " : " <<  request << std::endl;
	if (config->get_logs() == "full")
		std::cout << YELLOW << "[" << request.get_full_request().substr(0, 300) << "]" << DEF << std::endl;

	// L'objet client creer une reponse a partir de l'objet Request
	HTTP::Response	response = Response(config, &request);

	// On converti l'objet Response en string et on l'envoi

	std::string	response_string = response.to_string();

	//std::cout << response_string.size() << std::endl;
	count++;
	if (config->get_logs() == "count")
		std::cout << count << std::endl;
	if (response_string.empty())
		std::cerr << ERROR << "[Server.cpp] Empty response" << std::endl;
	else {
		size_t ret = sendBig(client_socket, response_string.c_str(), response_string.size());
		if (ret == (size_t)-1)
			std::cerr << ERROR << "[Server.cpp] send() : " << strerror(errno) << std::endl;
		else {
			if (config->get_logs() == "short" || config->get_logs() == "full")
				std::cout << SEND << inet_ntoa(addr.sin_addr) << " : " << response << " [" << ret << "]" << std::endl;
			if (config->get_logs() == "full")
				std::cout << GREEN << "[" << response_string.substr(0, 300).c_str() << "]" << DEF << std::endl;
		}
			
	}
}

void	Server::run( void ) {

	int									max_socket_so_far = 0;

	fd_set								current_sockets, ready_sockets;

	std::map<int, ConfigServer *>	clients_config;

	// Configure les ports et demarre l'ecoute
	_setup_server();

	// Initialise la liste des sockets
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

		timeout.tv_sec = 10;
    	timeout.tv_usec = 0;

		// Car select est destructeur
		ready_sockets = current_sockets;

		if (select(max_socket_so_far + 1, &ready_sockets, NULL, NULL, &timeout) < 0)
			throw std::runtime_error("[Server.cpp] select() : " + std::string(strerror(errno)));

		for (int i = 0; i <= max_socket_so_far; i++) {
			if (FD_ISSET(i, &ready_sockets)) {

				Server::listens::iterator it = _listens.find(i);

				if (it != _listens.end()) {

					// Nouvelle connexion, on l'ajoute a la liste des sockets
					int client_socket = _accept_connection(it->first);
					clients_config.insert(std::make_pair(client_socket, it->second));
					FD_SET(client_socket, &current_sockets);
					if (client_socket > max_socket_so_far) {
						max_socket_so_far = client_socket;
					}
				} else {
					
					// On manage la connexion au socket et le supprime de notre liste
					FD_CLR(i, &current_sockets);
					//FD_CLR(i, &ready_sockets);
					_handle_connexion(i, clients_config.find(i)->second);
					clients_config.erase(i);
					//usleep(2000);
					close(i);
				}
			}
		}
	}
}

} // namespace HTTP
