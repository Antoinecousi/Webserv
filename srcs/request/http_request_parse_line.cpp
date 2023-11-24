/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request_parse_line.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 06:39:46 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/04 10:54:18 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <cstdlib>
#include <cctype>
#include "request.hpp"
#include "response.hpp"

static int	http_request_parse_line_method(Request &request, Response &response, std::string method)
{
	if (method != "OPTIONS" && method != "GET" && method != "HEAD" && method != "POST" \
	&&  method != "PUT" && method != "DELETE" && method != "TRACE" && method != "CONNECT")
		return (set_status_code_and_return(response, "501"));
	request.method = method;
	return (0);
}

static void	decode_uri(std::string &uri)
{
	std::string::size_type	size;
	long					character;

	size = uri.size();
	for (std::string::size_type i = 0; i < size; ++i)
	{
		if (uri[i] == '%')
		{
			if (i + 2 < size)
			{
				character = strtol(uri.substr(i + 1, 2).c_str(), NULL, 16);
				if (character >= ' ' && character <= '~')
				{
					uri[i] = static_cast<char>(character);
					uri.erase(i + 1, 2);
					size -= 2;
				}
			}
		}
	}
}

static int	http_request_parse_line_uri(Request &request, Response &response, std::string uri)
{
	std::vector<std::string>	components;
	std::vector<std::string>	normalized_components;
	std::string::size_type		start;
	std::string::size_type		end;

	if (uri[0] != '/')
		return (set_status_code_and_return(response, "400"));
	if (uri.find('?') != std::string::npos)
	{
		request.query = uri.substr(uri.find('?') + 1);
		uri = uri.substr(0, uri.find('?'));
	}
	// Split uri into multiple components using '/' as delimiter
	start = 0;
	end = uri.find('/');
	while (end != std::string::npos)
	{
		if (start != end)
			components.push_back(uri.substr(start, end - start));
		start = end + 1;
		end = uri.find('/', start);
	}
	if (start < uri.size() && start != end)
		components.push_back(uri.substr(start));

	for (std::vector<std::string>::iterator it = components.begin(); it != components.end(); it++)
	{
		if (*it == ".")
			continue ;
		else if (*it == "..")
		{
			// Remove the previous component
			if (normalized_components.empty())
				return (set_status_code_and_return(response, "400"));
			normalized_components.pop_back();
		}
		else
		{
			normalized_components.push_back(*it);
		}
	}

	// Construct the uri
	uri.clear();
	for (std::vector<std::string>::const_iterator it = normalized_components.begin(); it != normalized_components.end(); it++)
		uri += '/' + *it;

	request.uri = uri.empty() ? "/" : uri;
	decode_uri(request.uri);
	return (0);
}

static int	http_request_parse_line_version(Request &request, Response &response, std::string http_protocol_version)
{
	std::string	http_protocol;
	std::string	http_version;
	std::string	http_version_major_number;
	std::string	http_version_minor_number;

	if (http_protocol_version.find('/') == std::string::npos)
		return (set_status_code_and_return(response, "400"));
	http_protocol = http_protocol_version.substr(0, http_protocol_version.find('/'));
	http_version = http_protocol_version.substr(http_protocol_version.find('/') + 1);
	if (http_protocol != "HTTP")
		return (set_status_code_and_return(response, "400"));
	if (http_version.find(".") == std::string::npos)
		return (set_status_code_and_return(response, "400"));

	if (http_version.find('.') == std::string::npos)
		return (set_status_code_and_return(response, "400"));
	if (http_version.size() <= http_version.find('.') - http_version.find_first_not_of("0"))
		return (set_status_code_and_return(response, "400"));
	http_version_major_number = http_version.substr(http_version.find_first_not_of("0"), http_version.find('.') - http_version.find_first_not_of("0"));
	if (http_version.size() <= http_version.find_first_not_of("0", http_version.find('.') + 1))
		return (set_status_code_and_return(response, "400"));
	http_version_minor_number = http_version.substr(http_version.find_first_not_of("0", http_version.find('.') + 1));
	if (http_version_major_number.empty() || http_version_major_number.find_first_not_of("0123456789") != std::string::npos)
		return (set_status_code_and_return(response, "400"));
	if (http_version_minor_number.empty() ||  http_version_minor_number.find_first_not_of("0123456789") != std::string::npos)
		return (set_status_code_and_return(response, "400"));
	if (http_version_major_number != "1" || http_version_minor_number != "1")
		return (set_status_code_and_return(response, "505"));

	request.version = http_protocol + "/" + http_version_major_number + "." + http_version_minor_number;
	return (0);
}

int	http_request_parse_line(Request &request, Response &response, std::string &line)
{
	std::vector<std::string>	tokens;
	std::string::size_type		start;
	std::string::size_type		end;

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
	if (tokens.size() != 3)
		return (set_status_code_and_return(response, "400"));

	if (http_request_parse_line_method(request, response, tokens[0]) < 0)
		return (-1);
	if (http_request_parse_line_uri(request, response, tokens[1]) < 0)
		return (-1);
	if (http_request_parse_line_version(request, response, tokens[2]) < 0)
		return (-1);
	return (0);
}
