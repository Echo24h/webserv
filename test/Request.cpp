#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

int request(char **argv) {

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Erreur lors de la création de la socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3002); // Port HTTP par défaut

    // Adresse IP ou nom de domaine du serveur cible
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Adresse invalide ou non supportée" << std::endl;
        return 1;
    }

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Erreur lors de la connexion au serveur" << std::endl;
        return 1;
    }

    // Lecture de la requête DELETE à partir du fichier delete.txt
    std::ifstream infile(argv[1]);
    std::string request;
    std::string line;
    while (std::getline(infile, line)) {
        request += line + "\r\n";
    }
    infile.close();

    // Envoi de la requête
    if (send(sockfd, request.c_str(), request.size(), 0) != request.size()) {
        std::cerr << "Erreur lors de l'envoi de la requête" << std::endl;
        return 1;
    }

    // Fermeture de la connexion
    close(sockfd);

    return 0;
}

int main(int argc, char **argv) {

    if (argc != 2 && argc != 3) {
        std::cerr << "Usage : ./request <request_file.txt>" << std::endl;
        std::cerr << "        ./request <request_file.txt> <nb_of_of_request>" << std::endl;
        return 1;
    }
    else {

        int nb_of_request = 1;

        if (argc == 3)
            nb_of_request = atoi(argv[2]);

        while (nb_of_request > 0) {
            if (request(argv) != 0)
                return 1;
            nb_of_request--;
        }
    }
    return 0;
}
