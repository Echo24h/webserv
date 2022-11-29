/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 17:09:21 by gborne            #+#    #+#             */
/*   Updated: 2022/11/29 15:29:25 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include <iostream>
# include <cstring>
# include <cstdlib>
# include <fstream>
# include <vector>

# include "../inc/Server.hpp"

class Config {

public:

	typedef	std::vector<std::string>	tokens;
	typedef	std::vector<Server>			servers;
	typedef	servers::iterator			iterator;
	typedef	servers::const_iterator		const_iterator;

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

	tokens		_tokenizeConfig( void ) const;
	tokens::iterator	_traitServLoc( Server * server, tokens::iterator it, tokens::iterator ite );
	tokens::iterator	_traitServParam( Server * server, tokens::iterator it );
	void		_initServers( tokens tokens );

	// Privates members

	const char *	_path;
	servers			_servers;

};

std::ostream &	operator<<( std::ostream & o, Config const & rhs );

#endif
