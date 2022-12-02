/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 03:27:59 by gborne            #+#    #+#             */
/*   Updated: 2022/12/02 18:52:31 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// http://vidalc.chez.com/lf/socket.html

#include "../inc/Server.hpp"

Server::Server( void ) { return ; }

char *	getHtml( const char * path ) {
	std::string		html;
	std::string		str;
	std::ifstream	ifs(path);
	char *			header = strdup("HTTP/1.1 200 OK\r\n\r\n");

	while (std::getline(ifs, str))
		html += str;

	char *	result = (char *)malloc(sizeof(char *) * (html.length() + strlen(header) + 1));

	strcpy(result, header);
	strcat(result, html.c_str());

	return result;
}

int	setup_server() {

	int	opt = 1;
	int server_socket;

	struct sockaddr_in my_addr;    /* Adresse */

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 1) {
		std::cerr << "[Server.cpp] socket() : " << strerror(errno) << std::endl;
		exit(1);
	}

	//set master socket to allow multiple connections , this is just a good habit, it will work without this
    if( setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

	my_addr.sin_family = AF_INET;         /* host byte order */
	my_addr.sin_port = htons(MYPORT);     /* short, network byte order */
	my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-remplissage avec mon IP */
	bzero(&(my_addr.sin_zero), 8);        /* zero pour le reste de struct */

	if (bind(server_socket, (struct sockaddr *)&my_addr, sizeof(sockaddr)) == -1) {
		std::cerr << "[Server.cpp] bind() : " << strerror(errno) << std::endl;
		exit(1);
	}

	if (listen(server_socket, BACKLOG) == -1) {
		std::cerr << "[Server.cpp] listen() : " << strerror(errno) << std::endl;
		exit(1);
	}
	return server_socket;
}

int	accept_connection( int server_socket ) {

	int	client_socket;

	socklen_t addrlen;

	struct sockaddr_in client_addr; /* Adresse du connecté  */

	addrlen = sizeof(client_addr);

	if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addrlen)) == -1)
		std::cerr << "[Server.cpp] accept() : " << strerror(errno) << std::endl;
	else
		std::cout << "Serveur: Reçu connection de " << inet_ntoa(client_addr.sin_addr) << std::endl;

	return client_socket;
}

void * handle_connection( int client_socket ) {

	/*char	buffer[BUFSIZ];
	ssize_t	bytes_read;
	int		msgsize = 0;
	char	actualpath[_PC_PATH_MAX+1];

	//read the client's message -- the name of the file to read
	while((bytes_read = read(client_socket, buffer+msgsize, sizeof(buffer) - msgsize - 1)) > 0) {
		msgsize += bytes_read;
		if (msgsize > BUFSIZ-1 || buffer[msgsize-1] == '\n')
			break;
	}
	if (bytes_read != 0)
		std::cerr << "[Server.cpp] read() : " << strerror(errno) << std::endl;
	buffer[msgsize-1] = 0;

	std::cout << "BUF = " << buffer << std::endl;
	fflush(stdout);

	//validity check
	if (realpath(buffer, actualpath) == NULL) {
		std::cout << "ERROR(bad_path)=" << buffer << std::endl;
		close(client_socket);
		return NULL;
	}

	//read file and send its contents to client
	FILE *fp = fopen(actualpath, "r");
	if (fp == NULL) {
		std::cout << "ERROR(open)=" << buffer << std::endl;
		close(client_socket);
		return NULL;
	}

	//read file contents and send them to client
	while((bytes_read = fread(buffer, 1, BUFSIZ, fp)) > 0) {
		write(client_socket, buffer, bytes_read);
	}
	close(client_socket);
	fclose(fp) ;
	std::cout << "closing connection" << std::endl;

	return NULL;*/

	if (fork() == 0) { // processus fils

		char * html = getHtml("www/index.html");

		if (send(client_socket, html, strlen(html), 0) == -1)
			std::cerr << "[Server.cpp] send() : " << strerror(errno) << std::endl;
		close(client_socket);
		exit(0);
	}
	close(client_socket);  // Le parent n'a pas besoin de cela

	// Nettoyage des processus fils
	while(waitpid(-1,NULL,WNOHANG) > 0);
	return NULL;
}

void	Server::run( void ) {

	int server_socket;  /* Ecouter server_socket, nouvelle connection sur client */

	server_socket = setup_server();

	int	max_sd = 0;
	int	sd;

	int	client_socket[30];
	bzero(client_socket, 30);
	int	max_clients = 30;
	int	new_socket;

	struct sockaddr_in address;
	int	addrlen;

	char	buffer[1025];

	fd_set	ready_sockets;

	//accept the incoming connection
    addrlen = sizeof(address);
    puts("Waiting for connections ...");

	while(1) {  /* main accept() loop */

		// initialize my current set

		//clear the socket set
		FD_ZERO(&ready_sockets);

		//add master socket to set
		FD_SET(server_socket, &ready_sockets);
		max_sd = server_socket;

		std::cout << "start" << std::endl;

		//add child sockets to set
        for (int i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
			sd = client_socket[i];

			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &ready_sockets);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
				max_sd = sd;
        }

		std::cout << "mid" << std::endl;

        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        int activity = select( max_sd + 1 , &ready_sockets , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

		std::cout << "OK" << std::endl;

        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(server_socket, &ready_sockets))
        {
            if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //inform user of socket number - used in send and receive commands
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

            //send new connection greeting message
			const char *message = "ECHO Daemon v1.0 \r\n";
            if( send(new_socket, message, strlen(message), 0) != (ssize_t)strlen(message) )
            {
                perror("send");
            }

            puts("Welcome message sent successfully");

            //add new socket to array of sockets
            for (int i = 0; i < max_clients; i++)
            {
                //if position is empty
				if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n" , i);

					break;
                }
            }
        }

        //else its some IO operation on some other socket :)
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET( sd , &ready_sockets))
            {
				int	bytes_read;
                //Check if it was for closing , and also read the incoming message
                if ((bytes_read = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    client_socket[i] = 0;
                }

                //Echo back the message that came in
                else
                {
                    //set the string terminating NULL byte on the end of the data read
                    buffer[bytes_read] = '\0';
                    send(sd , buffer , strlen(buffer) , 0 );
                }
            }
        }
		std::cout << "end" << std::endl;
	}
}
