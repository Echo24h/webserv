/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 20:01:30 by gborne            #+#    #+#             */
/*   Updated: 2022/12/02 20:29:48 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTHANDLER_HPP
# define RREQUESTHANDLER_HPP

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

# include "message.h"

class RequestHandler {

public:

	RequestHandler();

	void	trait_request( int client_socket );

};

#endif
