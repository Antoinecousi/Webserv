/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_duplicate.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 09:05:24 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:10:22 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <utility>
#include <string>
#include "Http_block.hpp"
#include "Server_block.hpp"
#include "Location_block.hpp"
#include "parsing.hpp"

int	is_duplicate_client_max_body_size(std::pair<e_context, void*> &context)
{
	if (context.first == HTTP)
		return (get_context<Http_block>(context).get_flag_client_max_body_size());
	else if (context.first == SERVER)
		return (get_context<Server_block>(context).get_flag_client_max_body_size());
	else
		return (get_context<Location_block>(context).get_flag_client_max_body_size());
}

int	is_duplicate_return(std::pair<e_context, void*> &context)
{
	if (context.first == SERVER)
		return (get_context<Server_block>(context).get_flag_return_());
	else
		return (get_context<Location_block>(context).get_flag_return_());
}

int	is_duplicate_root(std::pair<e_context, void*> &context)
{
	if (context.first == HTTP)
		return (get_context<Http_block>(context).get_flag_root());
	else if (context.first == SERVER)
		return (get_context<Server_block>(context).get_flag_root());
	else
		return (get_context<Location_block>(context).get_flag_root());
}

int	is_duplicate_autoindex(std::pair<e_context, void*> &context)
{
	if (context.first == HTTP)
		return (get_context<Http_block>(context).get_flag_autoindex());
	else if (context.first == SERVER)
		return (get_context<Server_block>(context).get_flag_autoindex());
	else
		return (get_context<Location_block>(context).get_flag_autoindex());
}
