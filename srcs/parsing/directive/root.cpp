/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   root.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acousini <acousini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:52:14 by spayeur           #+#    #+#             */
/*   Updated: 2023/11/24 15:24:27 by acousini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <string>
#include <limits.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <pwd.h>
#include "Http_block.hpp"
#include "Server_block.hpp"
#include "Location_block.hpp"
#include "parsing.hpp"

const std::string get_default_root(void)
{
    char exePath[4096];
    ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);

    if (len != -1)
    {
        exePath[len] = '\0';
        char* lastSlash = std::strrchr(exePath, '/');
        
        if (lastSlash != NULL)
        {
            *lastSlash = '\0';
			// std::cout << "Path for html execution files: " << std::string(exePath) + "/html" << std::endl;
            return std::string(exePath) + "/html";
        }
    }
	const struct passwd	*pws = getpwuid(geteuid());
	const std::string	username = !(pws) ? "root" : pws->pw_name;
	std::cout << "the program couldn't access the path, path to default is now : " << "/goinfre/" + username + "/webserv/html" << std::endl;
	return ("/goinfre/" + username + "/webserv/html");
}

int	parse_root(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	const std::string	working_directory = get_default_root();
	std::string			root;
	std::string			path;

	if (args[0][0] == '/')
		path = args[0];
	else
		path = working_directory + '/' + args[0];

	// Normalize path
	root = normalize_path(directive, l, path, true, true);
	if (root.empty())
		return (-1);
	if (root.size() >= PATH_MAX)
		return (too_long_path_after_resolution_error(directive, l, root.substr(0, 10) + "..."));

	if (context.first == HTTP)
	{
		Http_block	&http = get_context<Http_block>(context);

		http.root = root;
		http.set_flag_root(true);
	}
	else if (context.first == SERVER)
	{
		Server_block	&server = get_context<Server_block>(context);

		server.root = root;
		server.set_flag_root(true);
	}
	else
	{
		Location_block	&location = get_context<Location_block>(context);

		location.root = root;
		location.set_flag_root(true);
	}
	return (0);
}
