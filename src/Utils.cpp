/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 18:16:41 by gborne            #+#    #+#             */
/*   Updated: 2023/05/23 17:54:11 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Utils.hpp"

namespace HTTP
{

std::vector<std::string>	split ( const std::string & str, const std::string & delim ) {

	std::vector<std::string> tokens;
	std::string str_cpy = str;

	std::size_t pos = 0;
	std::string token;

	while ((pos = str_cpy.find(delim)) != (size_t)-1) {
		token = str_cpy.substr(0, pos);
		str_cpy.erase(0, pos + delim.length());

		tokens.push_back(token);
	}
	if (str_cpy.length() > 0)
		tokens.push_back(str_cpy);

	return tokens;
}

std::string	itoa( const int & num ) {
	std::string s;
	std::stringstream out;
	out << num;
	s = out.str();
	return s;
}

bool	is_number( const std::string & s )
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}


bool	file_exist( const std::string & path ) {
	struct stat st;
	if (stat(path.c_str(), &st) != 0) {
		return false;  // erreur de stat()
	}
	if ((st.st_mode & S_IFMT) == S_IFDIR) {
		return false;  // le chemin correspond à un dossier
	}
	std::ifstream file(path.c_str());
	return file.good();
}

std::string	get_key( const std::string & line, const std::string & separator ) {

	std::string	key;
	size_t		n;

	n = line.find(separator);
	if (n > 0)
		key = line.substr(0, n);
	return key;
}

std::string	get_value( const std::string & line, const std::string & separator ) {

	std::string	value;
	size_t		n;

	n = line.find(separator);
	n += separator.size();
	if (n > 0)
		value = line.substr(n, line.size() - n);
	return value;
}

std::string	get_extension( const std::string & real_path ) {

	std::string	extention;

	size_t	last_point = real_path.find_last_of('.');
	size_t	last_slash = real_path.find_last_of('/');

	if (last_point != (size_t)-1 || last_slash < last_point)
		extention = real_path.substr(last_point + 1, real_path.size() - last_point - 1);

	return extention;
}

std::string	get_current_dir( void ) {

	char * current_dir_name = getcwd(NULL, MAXPATHLEN);
	
	std::string current_dir(current_dir_name);

	free(current_dir_name);

	return current_dir;
}

std::string read_file( const std::string & path ) {

	std::string		str;
	std::ifstream	ifs(path.c_str());

	std::string		content;

	if (ifs.fail())
		std::cerr << ERROR << "[Utils.cpp] read_file() : file " << path << " not found" << std::endl;
	else {
		while (std::getline(ifs, str))
			content += str += '\n';
		ifs.close();
	}
	return content;
}

int	create_file( const std::string & file_name, const std::string & content ) {
	
	std::ofstream file(file_name.c_str());

	if (!file.is_open()) {
		std::cerr << ERROR << "[Utils.cpp] create_file() : can't open " << file_name << " : " << strerror(errno) << std::endl;
		return 1;
	}

	try {
		file << content;
		file.close();
	} catch ( const std::exception & e ) {
		file.close();
		std::remove(file_name.c_str());
		std::cerr << ERROR << "[Utils.cpp] create_file() : can't write on " << file_name << " : " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

bool is_string_greater_than_int_max( const std::string & number_string ) {

    const std::string int_max_string = "2147483647"; // Valeur maximale d'un int en C++98
	
    if (number_string.length() > int_max_string.length()) {
        return true;
    } else if (number_string.length() < int_max_string.length()) {
        return false;
    } else {
        return number_string > int_max_string;
    }
}

std::string remove_double_slashes( const std::string & path ) {
	
	size_t pos = 0;
	std::string new_path = path;

	while ((pos = new_path.find("//", pos)) != std::string::npos) {
		new_path.replace(pos, 2, "/");
	}
	return new_path;
}

ssize_t 					sendBig(int sockfd, const char * data, ssize_t size) {
    // Initialisez les données selon vos besoins

    int totalSent = 0;
    int remaining = size;
    int chunkSize = 10000;  // Taille de chaque morceau à envoyer

	if (remaining < chunkSize)
		chunkSize = remaining;

    while (remaining > 0) {
			
        int bytesSent = send(sockfd, data + totalSent, chunkSize, 0);
        if (bytesSent == -1) {
            // Gestion de l'erreur d'envoi
			if (errno != EAGAIN && errno != EWOULDBLOCK) {
				std::cerr << ERROR << "[Utils.cpp] sendBig() : Erreur send() : " << strerror(errno) << std::endl;
				break;
			}
        }

        totalSent += bytesSent;
        remaining -= bytesSent;

		if (remaining < chunkSize)
			chunkSize = remaining;
		//usleep(2000);
    }
	return (ssize_t)totalSent;
}

} // namespace HTTP
