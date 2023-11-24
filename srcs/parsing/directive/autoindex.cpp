/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:54:14 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:10:22 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <string>
#include "Http_block.hpp"
#include "Server_block.hpp"
#include "Location_block.hpp"
#include "parsing.hpp"

int	parse_autoindex(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::string	value;
	bool		autoindex;

	value = args[0];
	for (std::string::iterator it = value.begin(); it != value.end(); it++)
		*it = std::tolower(*it);
	if (value == "on")
		autoindex = true;
	else if (value == "off")
		autoindex = false;
	else
		return (invalid_value_error(directive, l, value));

	if (context.first == HTTP)
	{
		Http_block	&http = get_context<Http_block>(context);

		http.autoindex = autoindex;
		http.set_flag_autoindex(true);
	}
	else if (context.first == SERVER)
	{
		Server_block	&server = get_context<Server_block>(context);

		server.autoindex = autoindex;
		server.set_flag_autoindex(true);
	}
	else
	{
		Location_block	&location = get_context<Location_block>(context);

		location.autoindex = autoindex;
		location.set_flag_autoindex(true);
	}
	return (0);
}
