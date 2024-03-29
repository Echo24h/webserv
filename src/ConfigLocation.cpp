/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigLocation.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 19:37:23 by gborne            #+#    #+#             */
/*   Updated: 2023/06/01 14:40:53 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ConfigLocation.hpp"

namespace HTTP {

// CANONICAL FORM

ConfigLocation::ConfigLocation( void ) : _body_limit(INT_MAX) {
	return ; 
}

ConfigLocation::ConfigLocation( const ConfigLocation & src ) : _body_limit(INT_MAX) { *this = src; return ; }

ConfigLocation::~ConfigLocation() { return ; }

ConfigLocation &	ConfigLocation::operator=( const ConfigLocation & rhs ) {
	_name = rhs._name;
	_methods = rhs._methods;
	_index = rhs._index;
	_root = rhs._root;
	_params = rhs._params;
	_cgi = rhs._cgi;
	_body_limit = rhs._body_limit;
	return *this;
}

// SETTERS

// défini les paramètres de root et le nom de location
void	ConfigLocation::set_name( const std::string & name ) {

	std::string loc_name = name;

	if (loc_name[0] != '/')
		throw std::invalid_argument("[ConfigLocation.cpp] invalid location '" + name + "' in config file");
	
	if (loc_name.size() == 1 && loc_name == "/")
		this->_params = std::string("/*");
	else if (loc_name.size() > 1) {

		if (loc_name[loc_name.size() - 1] == '/') {
			loc_name = loc_name.substr(0, loc_name.size() - 1);
			this->_params = std::string("/");
		}
		else if (loc_name.size() > 2) {
			
			if (loc_name[loc_name.size() - 1] == '*' && loc_name[loc_name.size() - 2] == '/') {
				this->_params = std::string("/*");
				loc_name = loc_name.substr(0, loc_name.size() - 2);
			}
		}
	}
	_name = loc_name;
	return ;
}

void	ConfigLocation::new_method( const std::string & method ) {

	if (method != "DELETE" && method != "POST" && method != "GET" && method != "PUT")
		throw std::invalid_argument("[ConfigLocation.cpp] invalid method '" + method + "' in config file");
	_methods.push_back(method);
	return ;
}

void	ConfigLocation::set_index( const std::string & index ) { _index = index; return ; }

void	ConfigLocation::set_root( const std::string & root ) { _root = root; return ; }

void	ConfigLocation::set_body_limit( const std::string & body_limit ) {

	char		extension = *body_limit.rbegin();
	std::string	num;

	if (extension == 'M' || extension == 'K')
		num = body_limit.substr(0, body_limit.size() - 1);
	else
		num = body_limit;

	if (num.empty() || !is_number(num))
		throw std::invalid_argument("[ConfigServer.cpp] wrong body_limit");
	
	if (is_string_greater_than_int_max(num))
		throw std::invalid_argument("[ConfigServer.cpp] body_limit greather than INT_MAX");

	if (extension == 'M')
		_body_limit = atoi(num.c_str()) * 1000000;
	else if (extension == 'K')
		_body_limit = atoi(num.c_str()) * 1000;
	else
		_body_limit = atoi(body_limit.c_str());

	if (_body_limit < 2)
		throw std::invalid_argument("[ConfigServer.cpp] body_limit must be larger than 2");
	return ;
}

void	ConfigLocation::new_cgi( const std::string & extension, const std::string & path ) {
	_cgi.insert(std::make_pair(extension, path));
	return ;
}

// GETTERS

std::string	ConfigLocation::get_name( void ) const { return _name; }

ConfigLocation::methods		ConfigLocation::get_methods( void ) const { return _methods; }

std::string	ConfigLocation::get_index( void ) const { return _index; }

std::string	ConfigLocation::get_root( void ) const { return _root; }

std::string	ConfigLocation::get_params( void ) const { return _params; }

int			ConfigLocation::get_body_limit( void ) const { return _body_limit; }

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
	o << "	params  = " << rhs.get_params() << std::endl;
	o << "	body_limit = " << rhs.get_body_limit() << std::endl;
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
