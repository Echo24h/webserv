/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 19:37:23 by gborne            #+#    #+#             */
/*   Updated: 2023/03/15 20:54:40 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/ConfigLocation.hpp"

namespace HTTP {

// CANONICAL FORM

ConfigLocation::ConfigLocation( void ) { return ; }

ConfigLocation::ConfigLocation( const ConfigLocation & src ) { *this = src; return ; }

ConfigLocation::~ConfigLocation() { return ; }

ConfigLocation &	ConfigLocation::operator=( const ConfigLocation & rhs ) {
	_name = rhs._name;
	_methods = rhs._methods;
	_index = rhs._index;
	_root = rhs._root;
	_cgi = rhs._cgi;
	return *this;
}

// SETTERS

void	ConfigLocation::set_name( const std::string & name ) { _name = name; return ; }

void	ConfigLocation::new_method( const std::string & method ) {

	if (method != "DELETE" && method != "POST" && method != "GET")
		throw std::invalid_argument("[ConfigLocation.cpp] invalid method '" + method + "' in config file");
	_methods.push_back(method);
	return ;
}

void	ConfigLocation::set_index( const std::string & index ) { _index = index; return ; }

void	ConfigLocation::set_root( const std::string & root ) { _root = root; return ; }

void	ConfigLocation::new_cgi( const std::string & extension, const std::string & path ) {
	_cgi.insert(std::make_pair(extension, path));
	return ;
}

// GETTERS

std::string	ConfigLocation::get_name( void ) const { return _name; }

ConfigLocation::methods		ConfigLocation::get_methods( void ) const { return _methods; }

std::string	ConfigLocation::get_index( void ) const { return _index; }

std::string	ConfigLocation::get_root( void ) const { return _root; }

ConfigLocation::cgi			ConfigLocation::get_cgis( void ) const { return _cgi; }

std::string	ConfigLocation::get_cgi( const std::string & extension ) const {

	ConfigLocation::cgi::const_iterator it = _cgi.find("." + extension);

	if (it == _cgi.end())
		return std::string();
	return it->second;
}

// FUNCTIONS

bool	ConfigLocation::is_method( const std::string & method ) const {

	ConfigLocation::methods::const_iterator	it = _methods.begin();
	ConfigLocation::methods::const_iterator	ite = _methods.end();

	while (it != ite) {
		if (*it == method)
			return true;
		it++;
	}
	return false;
}

std::ostream &	operator<<( std::ostream & o, ConfigLocation const & rhs ) {
	o << "	name    = " << rhs.get_name() << std::endl;
	o << "	methods = ";
	ConfigLocation::methods methods = rhs.get_methods();
	ConfigLocation::methods::const_iterator	it_m = methods.begin();
	ConfigLocation::methods::const_iterator	ite_m = methods.end();
	while (it_m != ite_m) {
		o << *it_m;
		it_m++;
		if (it_m != ite_m)
			o << " ";
	}
	o << std::endl;
	o << "	index   = " << rhs.get_index() << std::endl;
	o << "	root    = " << rhs.get_root() << std::endl;
	o << "	cgi:" << std::endl;
	ConfigLocation::cgi cgi = rhs.get_cgis();
	ConfigLocation::cgi::const_iterator	it_c = cgi.begin();
	ConfigLocation::cgi::const_iterator	ite_c = cgi.end();
	while (it_c != ite_c) {
		o << "		" << it_c->first << " " << it_c->second;
		it_c++;
		if (it_c != ite_c)
			o << std::endl;
	}
	return o;
}

} // namespace HTTP
