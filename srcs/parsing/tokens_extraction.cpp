/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_extraction.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:36:08 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/11 12:14:19 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "parsing.hpp"

int	extract_tokens(std::ifstream &ifs, std::vector<std::string> &tokens, std::vector<std::string>::const_iterator &token_it, std::string::size_type &l)
{
	std::string				line;
	std::istringstream		iss;
	std::string				token;
	std::string::size_type	start;
	std::string::size_type	end;

	tokens.clear();
	std::getline(ifs, line);
	iss.str(line);
	l += 1;
	// Split the line into tokens (std::istringstream::operator>> use ' ', '\t' and '\n' as delimiters)
	while (iss >> token)
	{
		// Split the token using ";{}" as delimiters
		start = 0;
		end = token.find_first_of(";{}");
		while (end != std::string::npos)
		{
			if (start != end)
			{
				tokens.push_back(token.substr(start, end - start));
				start = end;
			}
			else
			{
				tokens.push_back(token.substr(start, 1));
				start = end + 1;
			}
			end = token.find_first_of(";{}", start);
		}
		if (start < token.size() && start != end)
			tokens.push_back(token.substr(start));
	}

	for (std::vector<std::string>::iterator token = tokens.begin(); token != tokens.end(); token++)
	{
		// Remove everything after a comment
		if ((*token)[0] == '#')
		{
			tokens.erase(token, tokens.end());
			break ;
		}
		if ((*token).find('#') != std::string::npos)
			return (unexpected_character_error('#', l));
	}
	if (!(ifs))
		return (-1);
	// Skip empty line and commented line
	if (tokens.empty())
	{
		if (extract_tokens(ifs, tokens, token_it, l) < 0)
			return (-1);
	}
	token_it = tokens.begin();
	return (0);
}
