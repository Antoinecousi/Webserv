/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:31:50 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:09:12 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stack>
#include <utility>
#include "Http_block.hpp"
#include "parsing.hpp"

void	parse_server(std::stack< std::pair<e_context, void *> > &contexts)
{
	Http_block	&http = get_context<Http_block>(contexts.top());

	http.server.push_back(Server_block());
	http.set_flag_server(true);
	contexts.push(std::pair<e_context, void *>(SERVER, &(http.server.back())));
}
