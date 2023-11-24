/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:34:26 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/23 14:26:26 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stack>
#include <utility>
#include <limits.h>
#include "Http_block.hpp"
#include "Server_block.hpp"
#include "parsing.hpp"

int	parse_location(std::stack< std::pair<e_context, void *> > &contexts, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	bool		equal_modifier;
	std::string	uri;

	if (args.size() == 1)
	{
		equal_modifier = false;
	}
	else
	{
		if (args[0] != "=")
			return (invalid_location_modifier_error(directive, l, args[0]));
		equal_modifier = true;
	}
	// Normalize path
	uri = normalize_path(directive, l, std::string(args[args.size() - 1]), true, true);
	if (uri.empty())
		return (-1);
	if (uri.size() >= PATH_MAX)
		return (too_long_path_after_resolution_error(directive, l, uri.substr(0, 10) + "..."));

	if (contexts.top().first == SERVER)
	{
		Server_block	&server = get_context<Server_block>(contexts.top());

		for (std::vector<Location_block>::const_iterator it = server.location.begin(); it != server.location.end(); it++)
		{
			if (uri == (*it).uri && equal_modifier == (*it).equal_modifier)
				return (duplicate_location_error(directive, l, args[args.size() - 1], uri));
		}
		server.location.push_back(Location_block(uri, equal_modifier));
		server.set_flag_location(true);
		contexts.push(std::pair<e_context, void *>(LOCATION, &(server.location.back())));
	}
	else
	{
		Location_block	&location = get_context<Location_block>(contexts.top());

		if (location.equal_modifier)
			return (location_cannot_be_inside_the_exact_location_error(directive, l, args[args.size() - 1], location.uri));
		if ((uri != location.uri) && (uri.find(location.uri) != 0 \
		|| uri.size() <= location.uri.size() || (location.uri != "/" && uri[location.uri.size()] != '/')))
			return (location_is_outside_location_error(directive, l, args[args.size() - 1], uri, location.uri));
		for (std::vector<Location_block>::const_iterator it = location.location.begin(); it != location.location.end(); it++)
		{
			if (uri == (*it).uri && equal_modifier == (*it).equal_modifier)
				return (duplicate_location_error(directive, l, args[args.size() - 1], uri));
		}
		location.location.push_back(Location_block(uri, equal_modifier));
		location.set_flag_location(true);
		contexts.push(std::pair<e_context, void *>(LOCATION, &(location.location.back())));
	}
	return (0);
}
