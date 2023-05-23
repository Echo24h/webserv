/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLocation.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 19:21:42 by gborne            #+#    #+#             */
/*   Updated: 2023/05/23 17:48:02 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGLOCATION_HPP
# define CONFIGLOCATION_HPP

# include <iostream>
# include <vector>
# include <map>

# include "Utils.hpp"

namespace HTTP {

class ConfigLocation {

public:

	typedef	std::vector<std::string>			methods;
	typedef	std::map<std::string, std::string>	cgi;

	// CANONICAL FORM

	ConfigLocation( void );

	ConfigLocation( const ConfigLocation & src );

	~ConfigLocation();

	ConfigLocation &	operator=( const ConfigLocation & rhs );

	// SETTERS

	void	set_name( const std::string & name );

	void	new_method( const std::string & method );

	void	set_index( const std::string & index );

	void	set_body_limit( const std::string & body_limit );

	void	set_root( const std::string & root );

	void	new_cgi( const std::string & extension, const std::string & path );

	// GETTERS

	std::string	get_name( void ) const;

	methods		get_methods( void ) const;

	std::string	get_index( void ) const;

	std::string	get_root( void ) const;

	std::string	get_params( void ) const;

	int			get_body_limit( void ) const;

	cgi			get_cgis( void ) const;

	std::string	get_cgi( const std::string & extension ) const;

	// FUNCTIONS

	bool	is_method( const std::string & method ) const;

private:

	std::string		_name;
	methods			_methods;
	std::string		_index;
	std::string		_root;
	std::string		_params;
	cgi				_cgi;
	int				_body_limit;
};

std::ostream &	operator<<( std::ostream & o, ConfigLocation const & rhs );

} // namespace HTTP

#endif
