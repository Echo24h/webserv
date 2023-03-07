#include <iostream>
#include <fstream>
#include <sstream>
#include <curl/curl.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Cannot open file " << argv[1] << std::endl;
        return 1;
    }

    CURL *curl;
    CURLcode res;
    std::string response;

    // Vérification de la valeur de retour de curl_easy_init
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Erreur lors de l'initialisation de curl" << std::endl;
        return 1;
    }

    // Vérification de la valeur de retour de curl_global_init
    CURLcode res_init = curl_global_init(CURL_GLOBAL_ALL);
    if (res_init != CURLE_OK) {
        std::cerr << "Erreur lors de l'initialisation globale de curl : " << curl_easy_strerror(res_init) << std::endl;
        return 1;
    }

    if(curl) {
        std::string method, url;
        input >> method >> url;

        // Vérification de la valeur de retour de curl_easy_setopt
        CURLcode res_setopt = curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        if (res_setopt != CURLE_OK) {
            std::cerr << "Erreur lors de la configuration de l'option CURLOPT_CUSTOMREQUEST : " << curl_easy_strerror(res_setopt) << std::endl;
            return 1;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // Définit une fonction de rappel pour stocker la réponse du serveur HTTP
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [curl](char *ptr, size_t size, size_t nmemb, std::string *userdata) -> size_t {
            char *escaped_ptr = curl_easy_escape(curl, ptr, size * nmemb);
            if (escaped_ptr == NULL) {
                return 0;
            }
            userdata->append(escaped_ptr);
            curl_free(escaped_ptr);
            return size * nmemb;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Vérification de la valeur de retour de curl_easy_perform
        CURLcode res_perform = curl_easy_perform(curl);

        if(res_perform != CURLE_OK)
            std::cerr << "Erreur lors de l'envoi de la requête : " << curl_easy_strerror(res) << std::endl;
        else
            std::cout << "Réponse du serveur : " << response << std::endl;

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return 0;
}
