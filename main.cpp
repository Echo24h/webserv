/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 16:30:46 by gborne            #+#    #+#             */
/*   Updated: 2022/12/19 23:33:17 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/Server.hpp"
#include "inc/Config.hpp"

int	main( int argc, char **argv ) {
	if (argc != 2)
		std::cout << "command: ./webserv <config.conf>" << std::endl;
	else {
		try {
			HTTP::Config config(argv[1]);
			//std::cout << config << std::endl;
			HTTP::Server server(&config);
			server.run();
		} catch(const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}

