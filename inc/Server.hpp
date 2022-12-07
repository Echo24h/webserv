/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:18:38 by gborne            #+#    #+#             */
/*   Updated: 2022/12/05 18:07:32 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "ConfigServer.hpp"
# include "Request.hpp"
# include "Response.hpp"

# include <iostream>
# include <fstream>
# include <cstdlib>

# include <unistd.h>
# include <string.h>
# include <errno.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

#define MYPORT 3490	/* Le port où les utilisateurs se connecteront */

#define BACKLOG 10	/* Nombre maxi de connections acceptées en file */

class Server {

public:

	Server( void );

	Server( ConfigServer * config );

	Server( const Server & src );

	~Server();

	Server &	operator=( const Server & rhs );

	void	run( void );

private:

	int		_setup_server() const;

	int		_accept_connection( int server_socket ) const;

	void	_handle_connexion( int client_socket ) const;

	ConfigServer *	_config;

};

#endif
