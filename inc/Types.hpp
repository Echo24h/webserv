/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Types.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 21:29:41 by gborne            #+#    #+#             */
/*   Updated: 2023/03/17 21:42:50 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_HPP
# define TYPES_HPP

# include <iostream>
# include <fstream>
# include <map>

# include "Utils.hpp"

namespace HTTP {

class Types {

public:

	typedef	std::map<std::string, std::string>	types;
	typedef	types::iterator						iterator;
	typedef	types::const_iterator				const_iterator;

	// CANONICAL FORM

	//Use file path "conf/default.types" for default construct
	Types( void ) {
		_construct("conf/mime.types");
		return ;
	}

	Types( const std::string & path ) {
		_construct(path);
		return ;
	}

	Types( const Types & src ) {
		*this = src;
		return ;
	}

	~Types() {
		return ;
	}

	Types &	operator=( const Types & rhs ) {
		_types = rhs._types;
		return *this;
	}

	// ITERATORS

	iterator		begin() { return _types.begin(); }

	iterator		end() { return _types.end(); }

	const_iterator	begin() const { return _types.begin(); }

	const_iterator	end() const { return _types.end(); }



	// GETTERS

	std::string	get_type( const std::string & extension ) const {
		types::const_iterator	it = _types.find(extension);

		if (it == _types.end())
			return std::string();
		return it->second;
	}

private:

	void	_construct( const std::string & path ) {

		std::ifstream	ifs(path.c_str());

		if (ifs.fail())
			std::cerr << ERROR << "[Types.hpp] file \"" << path << "\" not found";
		else {

			std::string str;

			while(std::getline(ifs, str)) {

				std::vector<std::string> args = split(str, " ");

				if (args.size() >= 2) {

					std::string	last_extension = *args.rbegin();

					if (*last_extension.rbegin() != ';') {
						std::cerr << ERROR << "expected ';' at \"" << last_extension << "\" in file \"" + path + "\"";
						_types.clear();
						return;
					}

					last_extension = last_extension.substr(0, last_extension.size() - 1);

					args.pop_back();
					args.push_back(last_extension);

					std::vector<std::string>::const_iterator	it = args.begin();
					std::vector<std::string>::const_iterator	ite = args.end();

					it++;
					while (it != ite) {
						_types.insert(std::make_pair(*it, *args.begin()));
						it++;
					}
				}
				else if (args.size() == 1) {
					std::cerr << ERROR << "Wrong line format in file \"" << path << "\"" << std::endl;
					std::cerr << "	valid line example: \"ext/html   html htm shtml;\" (separate with spaces only)" << std::endl;
				}
			}
			ifs.close();
		}
		return ;
	}

	types	_types;

};

/*std::ostream &	operator<<( std::ostream & o, const Types & rhs ) {
	Types::types::const_iterator	it = rhs.begin();
	Types::types::const_iterator	ite = rhs.end();
	while (it != ite) {
		o << it->first << " => " << it->second;
		it++;
		if(it != ite)
			o << std::endl;
	}
	return o;
}*/

} // namespace HTTP

#endif
