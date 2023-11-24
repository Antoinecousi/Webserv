/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:29:39 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:09:12 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stack>
#include <utility>
#include "Http_block.hpp"
#include "parsing.hpp"

void	parse_http(std::stack< std::pair<e_context, void *> > &contexts, Http_block &http)
{
	contexts.push(std::pair<e_context, void *>(HTTP, &http));
}
