/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:27:59 by gborne            #+#    #+#             */
/*   Updated: 2022/11/30 04:08:18 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// http://vidalc.chez.com/lf/socket.html
 
#include "../inc/Server.hpp"

void	run( void ) {

	int sockfd, new_fd;  /* Ecouter sock_fd, nouvelle connection sur new_fd */
        struct sockaddr_in my_addr;    /* Adresse */
        struct sockaddr_in their_addr; /* Adresse du connecté  */
        int sin_size;

        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }

        my_addr.sin_family = AF_INET;         /* host byte order */
        my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
        my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-remplissage avec mon IP */
        bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */

        if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        	perror("bind");
        	exit(1);
        }

        if (listen(sockfd, BACKLOG) == -1) {
            perror("listen");
            exit(1);
        }

        while(1) {  /* main accept() loop */
            sin_size = sizeof(struct sockaddr_in);
            if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, 0)) == -1) {
                perror("accept");
                continue;
            }
            std::cout << "serveur: Reçu connection de " << inet_ntoa(their_addr.sin_addr) << std::endl;
            if (!fork()) { /* processus fils */
                if (send(new_fd, "Hello, world!\n", 14, 0) == -1)
                    perror("send");
                close(new_fd);
                exit(0);
            }
            close(new_fd);  /* Le parent n'a pas besoin de cela */

            while(waitpid(-1,NULL,WNOHANG) > 0); /* Nettoyage des processus fils */
        }

}