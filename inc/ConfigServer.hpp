/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 16:20:08 by gborne            #+#    #+#             */
/*   Updated: 2023/05/23 19:21:00 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

# include "Utils.hpp"

# include <iostream>
# include <vector>

# include "ConfigLocation.hpp"
# include "Types.hpp"

namespace HTTP {

class ConfigServer {

public:

	typedef ConfigLocation						location;
	typedef std::vector<location>				locations;
	typedef locations::iterator					iterator;
	typedef locations::const_iterator			const_iterator;

	typedef Types								types;
	typedef std::map<std::string, std::string>	cookies;

	// CANONICAL FORM

	ConfigServer( void );

	ConfigServer( const std::string & host, const int & port, const std::string & error_path  );

	ConfigServer( ConfigServer const & src );

	~ConfigServer();

	ConfigServer &	operator=( ConfigServer const & rhs );

	// ITERATORS (locations)

	iterator		begin( void );

	iterator		end( void );

	const_iterator	begin( void ) const;

	const_iterator	end( void ) const;

	// SETTERS

	void	set_debug( const std::string & debug );

	void	set_host( const std::string & host );

	void	set_port( const int & port );

	void	set_server_name( const std::string & server_name );

	void	set_error_path( const std::string & error_path );

	void	set_body_limit( const std::string & body_limit );

	void	new_location( location & location );

	// GETTERS

	cookies *	get_cookies( void ) const;

	bool		get_debug( void ) const;

	std::string	get_host( void ) const;

	int			get_port( void ) const;

	std::string	get_server_name( void ) const;

	std::string	get_error_path( void ) const;

	int			get_body_limit( void ) const;

	location	* get_location( const std::string & virtual_path );

	locations	get_locations( void );

	std::string	get_type( const std::string & extension ) const;

private:

	bool				_debug;
	std::string			_host;
	int					_port;
	std::string			_server_name;
	std::string			_error_path;
	int					_body_limit;
	locations			_locations;
	types				_types;
};

std::ostream &	operator<<( std::ostream & o, ConfigServer const & rhs );

} // namespace HTTP

#endif
