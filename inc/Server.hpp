/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:18:38 by gborne            #+#    #+#             */
/*   Updated: 2022/11/30 17:51:40 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <stdio.h>
# include <errno.h>
# include <iostream>
# include <fstream>
# include <cstdlib>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/wait.h>

#define MYPORT 3490	/* Le port où les utilisateurs se connecteront */

#define BACKLOG 10	/* Nombre maxi de connections acceptées en file */

class Server {

public:

	Server( void );

	void	run( void );

};

#endif
