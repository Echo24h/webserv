/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:18:44 by gborne            #+#    #+#             */
/*   Updated: 2022/11/30 15:33:34 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_CPP
# define CLIENT_CPP

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <netdb.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <sys/socket.h>

#define PORT 3490 /* Le port où le client se connectera */

#define MAXDATASIZE 100 /* Tampon d'entrée */

class Client {

public:

	void    run( void );

};

#endif
