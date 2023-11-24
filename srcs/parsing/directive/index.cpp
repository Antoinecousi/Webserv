/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:55:24 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:10:23 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include <limits.h>
#include "Http_block.hpp"
#include "Server_block.hpp"
#include "Location_block.hpp"
#include "parsing.hpp"

int	parse_index(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::vector<std::string>	pages;
	std::string					page;

	// Get pages
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		// Normalize page
		page = normalize_path(directive, l, *it, false, true);
		if (page.empty())
			return (-1);
		if (page.size() >= PATH_MAX)
			return (too_long_path_after_resolution_error(directive, l, page.substr(0, 10) + "..."));
		if (!(page.empty()) && std::find(pages.begin(), pages.end(), page) == pages.end())
			pages.push_back(page);
	}

	if (context.first == HTTP)
	{
		Http_block	&http = get_context<Http_block>(context);

		for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
		{
			if (std::find(http.index.begin(), http.index.end(), *it) == http.index.end())
				http.index.push_back(*it);
		}
		http.set_flag_index(true);
	}
	else if (context.first == SERVER)
	{
		Server_block	&server = get_context<Server_block>(context);

		for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
		{
			if (std::find(server.index.begin(), server.index.end(), *it) == server.index.end())
				server.index.push_back(*it);
		}
		server.set_flag_index(true);
	}
	else
	{
		Location_block	&location = get_context<Location_block>(context);

		for (std::vector<std::string>::const_iterator it = pages.begin(); it != pages.end(); it++)
		{
			if (std::find(location.index.begin(), location.index.end(), *it) == location.index.end())
				location.index.push_back(*it);
		}
		location.set_flag_index(true);
	}
	return (0);
}
