/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 18:16:41 by gborne            #+#    #+#             */
/*   Updated: 2022/12/20 22:48:13 by gborne           ###   ########.fr       */
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

	char * current_dir_name = get_current_dir_name();

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

} // namespace HTTP
