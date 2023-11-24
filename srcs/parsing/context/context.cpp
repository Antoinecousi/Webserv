/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   context.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:27:42 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/10 10:49:01 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <vector>
#include <string>
#include <limits.h>
#include "parsing.hpp"

static int	get_context_arguments(const std::string &directive, std::vector<std::string> &args, const std::vector<std::string> &tokens, std::vector<std::string>::const_iterator &token, const std::string::size_type l)
{
	// Get context arguments
	while (token != tokens.end() && *token != "{")
	{
		if (*token == ";" || *token == "}")
			return (directive_has_no_opening_error(directive, l));
		// Check argument size
		if ((*token).size() >= PATH_MAX)
			return (too_long_argument_error(directive, l, (*token).substr(0, 10) + "..."));
		args.push_back(*token);
		token++;
	}
	return (0);
}

int	parse_context(std::ifstream &ifs, std::string &directive, std::vector<std::string> &args, e_context context, size_t allowed_contexts, std::vector<std::string> &tokens, std::vector<std::string>::const_iterator &token, std::string::size_type l)
{
	directive = *token++;
	// Check context validity
	if (!(context & allowed_contexts))
		return (directive_not_allowed_here_error(directive, l));
	if (get_context_arguments(directive, args, tokens, token, l) < 0)
		return (-1);
	while (token == tokens.end())
	{
		if (extract_tokens(ifs, tokens, token, l) < 0)
			return (unexpected_end_of_file_error(l));
		if (get_context_arguments(directive, args, tokens, token, l) < 0)
			return (-1);
	}
	return (0);
}
