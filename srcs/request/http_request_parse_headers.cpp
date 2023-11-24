/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request_parse_headers.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 06:39:46 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/04 10:49:31 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <cctype>
#include "request.hpp"
#include "response.hpp"

int	http_request_parse_headers(Request &request, Response &response, std::string &line)
{
	std::vector<std::string>	tokens;
	std::string::size_type		start;
	std::string::size_type		end;
	std::string					value;

	if (std::isspace(line[0]) || line.find_first_of("\f\t\v") != std::string::npos)
		return (set_status_code_and_return(response, "400"));
	start = 0;
	end = line.find_first_of(" \r");
	while (end != std::string::npos)
	{
		if (line[end] == '\r' && line.find_first_not_of("\r", end + 1) != std::string::npos)
			return (set_status_code_and_return(response, "400"));
		tokens.push_back(line.substr(start, end - start));
		start = line.find_first_not_of(" \r", end);
		end = line.find_first_of(" \r", start);
	}
	if (start != std::string::npos)
		tokens.push_back(line.substr(start));
	if (!(tokens.empty()) && tokens[0].find(':') != std::string::npos)
	{
		if (!(tokens[0].size() <= tokens[0].find(':') + 1))
			tokens.insert(tokens.begin() + 1, tokens[0].substr(tokens[0].find(':') + 1));
		tokens[0] = tokens[0].substr(0, tokens[0].find(':'));
	}

	// Normalize tokens[0]
	tokens[0][0] = std::toupper(tokens[0][0]);
	for (std::string::size_type i = 1; i < tokens[0].size(); i++)
	{
		if (tokens[0][i - 1] == '-')
			tokens[0][i] = std::toupper(tokens[0][i]);
		else
			tokens[0][i] = std::tolower(tokens[0][i]);
	}

	// Check that there are no duplicates
	if (tokens[0] == "Transfer-Encoding" \
	||  tokens[0] == "Authorization" \
	||  tokens[0] == "Expect" \
	||  tokens[0] == "Host" \
	||  tokens[0] == "If-Match" \
	||  tokens[0] == "If-Modified-Since" \
	||  tokens[0] == "If-None-Match" \
	||  tokens[0] == "If-Range" \
	||  tokens[0] == "If-Unmodified-Since" \
	||  tokens[0] == "Content-Length" \
	||  tokens[0] == "Content-Range")
	{
		if (request.headers.count(tokens[0]))
			return (set_status_code_and_return(response, "400"));
	}

	if (tokens[0] == "Host")
	{
		// Check number of arguments
		if (tokens.size() != 2)
			return (set_status_code_and_return(response, "400"));
	}
	if (tokens[0] == "Content-Length")
	{
		// Check number of arguments
		if (tokens.size() != 2)
			return (set_status_code_and_return(response, "400"));
		tokens[1].erase(0, tokens[1].find_first_not_of("0"));
		// Check value limit
		if (tokens[1].size() > 19 || (tokens[1].size() == 19 && tokens[1].compare("9223372036854775807") > 0))
			return (set_status_code_and_return(response, "400"));
	}

	value = std::string();
	for (std::vector<std::string>::const_iterator it = tokens.begin() + 1; it != tokens.end(); it++)
	{
		if (value.empty())
			value = *it;
		else
			value += " " + *it;
	}
	request.headers.insert(std::pair<std::string, std::string>(tokens[0], value));
	return (0);
}
