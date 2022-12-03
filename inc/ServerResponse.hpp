/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerResponse.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/02 23:32:18 by gborne            #+#    #+#             */
/*   Updated: 2022/12/03 03:01:06 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERRESPONSE_HPP
# define SERVERRESPONSE_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <sstream>

# include "../inc/ConfigServer.hpp"

class ServerResponse {

public:

    ServerResponse( void );

    ServerResponse( const ConfigServer & config );

    //return content of the file "path"
    std::string content( const char * path ) const;

    //return html reponse whit the root of the file "path"
    std::string format( std::vector<std::string> tokens ) const;

    std::string get( std::string & recv ) const;

private:

    bool    _check( std::vector<std::string> tokens ) const;

    ConfigServer _config;

};

#endif