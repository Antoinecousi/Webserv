/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_name.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:42:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:09:25 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <algorithm>
#include <string>
#include "Server_block.hpp"
#include "parsing.hpp"

int	parse_server_name(std::pair<e_context, void*> &context, std::vector<std::string> &args)
{
	std::vector<std::string>	server_name(args.begin(), args.end());

	Server_block	&server = get_context<Server_block>(context);
	for (std::vector<std::string>::const_iterator it = server_name.begin(); it != server_name.end(); it++)
	{
		if (std::find(server.server_name.begin(), server.server_name.end(), *it) == server.server_name.end())
			server.server_name.push_back(*it);
	}
	server.set_flag_server_name(true);
	return (0);
}
