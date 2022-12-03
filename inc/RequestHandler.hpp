/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 20:01:30 by gborne            #+#    #+#             */
/*   Updated: 2022/12/03 02:59:55 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include <stdio.h>
# include <errno.h>
# include <fstream>
# include <cstdlib>
# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/wait.h>

# include <iostream>
# include <sstream>
# include <utility>
# include <map>

# include "message.h"
# include "ServerResponse.hpp"
# include "../inc/ConfigServer.hpp"

class RequestHandler {

public:

	RequestHandler( void );

	RequestHandler( const ConfigServer & config );

	void	trait_request( int client_socket );

private:
	
	ServerResponse _response;

};

#endif
