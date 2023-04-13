/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:18:38 by gborne            #+#    #+#             */
/*   Updated: 2023/04/11 17:45:57 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Config.hpp"
# include "Request.hpp"
# include "Response.hpp"

# include <iostream>
# include <fstream>
# include <cstdlib>
# include <map>

# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>

#define BACKLOG 10	/* Nombre maxi de connections acceptées en file */

namespace HTTP {

class Server {

public:

	typedef	std::map<int, ConfigServer *>	listens;

	// CANONICAL FORM

	Server( Config * config );

	Server( const Server & src );

	~Server();

	Server &	operator=( const Server & rhs );

	// FUNCTIONS

	void	run( void );

private:

	void	_setup_server( void );

	int		_accept_connection( const int & server_socket ) const;

	void	_handle_connexion( const int & client_socket, ConfigServer * config ) const;

	Config *	_config;
	listens		_listens;

};

} // namespace HTTP

#endif
