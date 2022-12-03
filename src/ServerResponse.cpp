/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 23:34:37 by gborne            #+#    #+#             */
/*   Updated: 2022/12/03 04:43:43 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ServerResponse.hpp"

ServerResponse::ServerResponse( void ) : _config(ConfigServer()) {
    return ;
}

ServerResponse::ServerResponse( const ConfigServer & config ) : _config(config) {
    return ;
}

std::string ServerResponse::content( const char * path ) const {

    std::string		str;
	std::ifstream	ifs(path);

    std::string     content;

	while (std::getline(ifs, str))
		content += str;

    return content;
}

std::string	ServerResponse::format( std::vector<std::string> tokens ) const {

    std::string     content = this->content("www/index.html");
	std::string		html = "HTTP/1.1 200 OK\r\n";
    html += "Content-Type: " + tokens.at(1) + "\r\n";
    html += "Content-Length: " + std::to_string(content.size()) + "\r\n\n";
    html += content;
    html += "\r\n";
	return html;
}

std::vector<std::string> tokenize( std::string & recv ) {

    std::vector<std::string> tokens;

    std::stringstream	stream(recv);
	std::string 		cursor;

    while (getline(stream, cursor, ' '))
        tokens.push_back(cursor);
    
    return tokens;
}

bool    ServerResponse::_check( std::vector<std::string> tokens ) const {

    std::cout << "<" << tokens.at(0) << ">" << std::endl;
    std::cout << "<" << tokens.at(1) << ">" << std::endl;
    std::cout << "<" << tokens.at(2) << ">" << std::endl;
    std::cout << "<" << tokens.size() << ">" << std::endl;

    if(tokens.size() != 3)
        return false;

    //ERROR ?
    //if (tokens.at(2) != "HTTP/1.1")
    //    return false;

    ConfigServer::const_iterator it = _config.begin();
    ConfigServer::const_iterator ite = _config.end();

    std::cout << _config << std::endl;

    if (it == ite)
        return false;
    else
        std::cout << "<" << it->name << "> <" << tokens.at(1) << ">" << std::endl;

    /*while (it != ite) {

        std::cout << "<" << it->name << "> <" << tokens.at(1) << ">" << std::endl;

        if (it->name == tokens.at(1)) {

            std::vector<std::string>::iterator methods_it = it.methods.begin();
            std::vector<std::string>::iterator methods_ite = it.methods.end();

            while (methods_it != methods_ite) {

                std::cout << "<" << *methods_it << "> <" << tokens.at(0) << ">" << std::endl;

                if (*methods_it == tokens.at(0))
                    return true;

                methods_it++;
            }
        }
        it++;
    }*/
    return false;
}

std::string ServerResponse::get( std::string & recv ) const {

    std::vector<std::string> tokens = tokenize(recv);

    if (!_check(tokens))
        return std::string("");
        
    return format(tokens);
}