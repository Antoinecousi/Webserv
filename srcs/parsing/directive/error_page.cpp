/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_page.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:43:42 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:10:22 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <set>
#include <utility>
#include <string>
#include "Http_block.hpp"
#include "Server_block.hpp"
#include "Location_block.hpp"
#include "parsing.hpp"

int	parse_error_page(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const std::string::size_type l)
{
	const std::set<std::string>	codes(args.begin(), args.end() - 1);
	const std::string			page = args[args.size() - 1];
	std::map<int, std::string>	error_page;

	for (std::set<std::string>::const_iterator it = codes.begin(); it != codes.end(); it++)
	{
		// Check error codes validity
		if ((*it).find_first_not_of("0123456789") != std::string::npos)
				return (invalid_value_error(directive, l, *it));
		// Check error codes limit
		if (atoi((*it).c_str()) < 300 || atoi((*it).c_str()) > 599)
			return (value_must_be_between_error(directive, l, *it, 300, 599, ""));
		error_page.insert(std::pair<int, std::string>(atoi((*it).c_str()), page));
	}

	if (context.first == HTTP)
	{
		Http_block	&http = get_context<Http_block>(context);

		http.error_page.insert(error_page.begin(), error_page.end());
		http.set_flag_error_page(true);
	}
	else if (context.first == SERVER)
	{
		Server_block	&server = get_context<Server_block>(context);

		server.error_page.insert(error_page.begin(), error_page.end());
		server.set_flag_error_page(true);
	}
	else
	{
		Location_block	&location = get_context<Location_block>(context);

		location.error_page.insert(error_page.begin(), error_page.end());
		location.set_flag_error_page(true);
	}
	return (0);
}
