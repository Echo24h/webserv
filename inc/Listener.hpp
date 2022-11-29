/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 17:06:41 by gborne            #+#    #+#             */
/*   Updated: 2022/11/29 17:13:31 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENER_HPP
# define LISTENER_HPP

# include "Server.hpp"

class Listener {

public:

	// CANONICAL FORM

	Listener( void );

	Listener( Server & server );

	Listener( Listener const & src );

	~Listener();

	Listener &	operator=( Listener const & rhs );

	// FUNCTIONS

	bool	init( void );

	bool	run( void );

private:

	Server &	_server;

};

#endif
