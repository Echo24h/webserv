/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gborne <gborne@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/27 16:30:46 by gborne            #+#    #+#             */
/*   Updated: 2022/11/29 15:57:02 by gborne           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "inc/Config.hpp"

int	main( int argc, char **argv ) {
	if (argc != 2)
		std::cout << "command: ./webserv [configuration file]" << std::endl;
	else {
		try {
			Config config(argv[1]);
			//std::cout << config << std::endl;
		} catch(const std::exception& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}
