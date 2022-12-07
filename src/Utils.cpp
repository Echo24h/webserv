/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 18:16:41 by gborne            #+#    #+#             */
/*   Updated: 2022/12/07 20:21:49 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Utils.hpp"

namespace HTTP
{

std::vector<std::string>	split ( const std::string & str, const std::string & delim ) {

	std::vector<std::string> tokens = std::vector<std::string>();
	std::string strCopy = str;

	std::size_t pos = 0;
	std::string token;

	while ((pos = strCopy.find(delim)) != std::string::npos)
	{
		token = strCopy.substr(0, pos);
		strCopy.erase(0, pos + delim.length());

		tokens.push_back(token);
	}

	if (strCopy.length() > 0)
	{
		tokens.push_back(strCopy);
	}

	return tokens;
}

std::string	get_key( const std::string & line ) {

	std::string	key;
	size_t		n;

	n = line.find(':');
	if (n > 0)
		key = line.substr(0, n);
	return key;
}

std::string	get_value( const std::string & line ) {

	std::string	value;
	size_t		n;

	n = line.find(": ");
	if (n > 0)
		value = line.substr(n + 2, line.size() - (n + 2));
	return value;
}

bool	file_exist( const std::string & path ) {
	if (FILE *file = fopen(path.c_str(), "r")) {
		fclose(file);
		return true;
	} else {
		return false;
	}
}

} // namespace HTTP
