/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   limit_except.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:48:37 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:10:23 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include "Location_block.hpp"
#include "parsing.hpp"

int	parse_limit_except(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const size_t l)
{
	std::vector<std::string>	http_methods;

	// Get http_methods
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		// Check method validity
		if (*it != "GET" \
		&&	*it != "POST" \
		&&	*it != "DELETE")
			return (invalid_method_error(directive, l, *it));
		if (!((*it).empty()) && std::find(http_methods.begin(), http_methods.end(), *it) == http_methods.end())
			http_methods.push_back(*it);
	}

	Location_block	&location = get_context<Location_block>(context);
	for (std::vector<std::string>::const_iterator it = http_methods.begin(); it != http_methods.end(); it++)
	{
		if (std::find(location.limit_except.begin(), location.limit_except.end(), *it) == location.limit_except.end())
			location.limit_except.push_back(*it);
	}
	location.set_flag_limit_except(true);
	return (0);
}
