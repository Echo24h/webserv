/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 18:16:41 by gborne            #+#    #+#             */
/*   Updated: 2022/12/13 16:25:47 by gborne           ###   ########.fr       */
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

bool	file_exist( const std::string & path ) {
	std::ifstream	ifs(path.c_str());
	if (ifs.fail())
		return false;
	else {
		std::string	str;
		std::getline(ifs, str);
		ifs.close();
		// if str is empty, is probably a directory
		if (str.empty())
			return false;
		else
			return true;
	}
}

std::string	get_extension( const std::string & real_path ) {

	std::string	extention;

	size_t	last_point = real_path.find_last_of('.');
	size_t	last_slash = real_path.find_last_of('/');

	if (last_point != (size_t)-1 || last_slash < last_point)
		extention = real_path.substr(last_point + 1, real_path.size() - last_point - 1);
	else
		std::cerr << ERROR << "[ConfigServer.cpp] get_extension() : extention doesn't exist" << std::endl;

	return extention;
}

std::string	get_current_dir( void ) {

	char * current_dir_name = get_current_dir_name();

	if (current_dir_name == NULL)
		std::cerr << ERROR << "[CGI.cpp] get_current_dir() : invalid environment variable 'PWD'" << std::endl;

	std::string current_dir(current_dir_name);

	free(current_dir_name);

	return current_dir;
}

std::string	html_basic_style() {

	std::stringstream	ss;

	ss << "<style>\n";
	ss << "* {\n";
	ss << "margin: 0;\n";
	ss << "padding: 0;\n";
	ss << "}\n";
	ss << ".container {\n";
	ss << "width: 100%;\n";
	ss << "height: 100vh;\n";
	ss << "display: flex;\n";
	ss << "flex-direction: column;\n";
	ss << "justify-content: center;\n";
	ss << "align-items: center;\n";
	ss << "background-color: black;\n";
	ss << "}\n";
	ss << "#error {\n";
	ss << "color: red;\n";
	ss << "text-align: center;\n";
	ss << "}\n";
	ss << "p {\n";
	ss << "color: #EAEAEA;\n";
	ss << "text-align: center;\n";
	ss << "}\n";
	ss << "</style>\n";
	return ss.str();
}

std::string	html_header( void ) {

	std::stringstream	ss;
	ss << "<!DOCTYPE html> <html lang=\"en\">\n";
	ss << "<head>\n";
	ss << "<meta charset=\"UTF-8\">\n";
	ss << "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
	ss << "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
	ss << "</head>\n";
	ss << "<body>\n";
	ss << html_basic_style();
	ss << "<div class=\"container\">\n";
	return ss.str();
}

std::string	html_footer( void ) {
	std::stringstream	ss;
	ss << "</div>\n";
	ss << "</body>\n";
	ss << "</html>\n";
	return ss.str();
}

} // namespace HTTP
