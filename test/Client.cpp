/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:13:59 by gborne            #+#    #+#             */
/*   Updated: 2022/11/30 15:42:14 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

int	main ( int 	argc, char ** argv ) {
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct hostent *he;
	struct sockaddr_in their_addr; /* Adresse de celui qui se connecte */

	if (argc != 2) {
		fprintf(stderr,"usage: client hostname\n");
		exit(1);
	}

	if ((he=gethostbyname(argv[1])) == NULL) {  /* Info de l'hôte */
		herror("gethostbyname");
		exit(1);
	}

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(PORT);    /* short, network byte order */
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	bzero(&(their_addr.sin_zero), 8);     /* zero pour le reste de struct */

	if (connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}

	if ((numbytes=recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	buf[numbytes] = '\0';

	printf("Reçu: %s",buf);

	close(sockfd);

	return 0;
}
