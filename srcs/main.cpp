/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:21:22 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/04 19:07:36 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include "Http_block.hpp"
#include "parsing.hpp"
#include "server.hpp"

int	main(int argc, char **argv)
{
	Http_block	http;
	std::string	configuration_file;

	if (argc > 2)
	{
		std::cerr << "webserv: too many arguments" << std::endl;
		return (1);
	}
	if (argc == 1)
		configuration_file = "conf/default.conf";
	else
		configuration_file = argv[1];
	if (parse(http, configuration_file) < 0)
		return (1);
	server_loop(http);
	return (0);
}
