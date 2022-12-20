/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 17:09:21 by gborne            #+#    #+#             */
/*   Updated: 2022/12/18 17:34:02 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ConfigServer.hpp"

# include <iostream>
# include <cstring>
# include <cstdlib>
# include <fstream>
# include <vector>

namespace HTTP {

class Config {

public:

	typedef	std::vector<ConfigServer>	servers;

	typedef	servers::iterator			iterator;
	typedef	servers::const_iterator		const_iterator;

	typedef	std::vector<std::string>	tokens;

	// CANONICAL FORM

	Config( void );

	Config( const char * path );

	Config( Config const & src );

	~Config();

	Config &	operator=( Config const & rhs );

	// ITERATORS

	iterator		begin( void );
	iterator		end( void );
	const_iterator	begin( void ) const;
	const_iterator	end( void ) const;

private:

	// Parse Config to Servers functions

	tokens					_tokenize( void ) const;
	tokens::const_iterator	_trait_serv_loc( ConfigServer * server, tokens::const_iterator & it, tokens::const_iterator & ite );
	tokens::const_iterator	_trait_serv_param( ConfigServer * server, tokens::const_iterator & it );
	void					_init_serv( const tokens & tokens );

	// Privates members

	const char *	_path;
	servers			_servers;

};

std::ostream &	operator<<( std::ostream & o, Config const & rhs );

} // namespace HTTP

#endif
