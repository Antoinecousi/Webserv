/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 19:41:26 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/04 16:08:04 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <algorithm>
#include "response.hpp"

static void	add_allow_header(const Location_block &location, Response &response)
{
	std::string	methods;

	if (!(location.limit_except.empty()))
		methods = *(location.limit_except.begin());
	for (std::vector<std::string>::const_iterator it = location.limit_except.begin() + 1; it != location.limit_except.end(); it++)
		methods += ", " + *it;
	response.headers.push_front(std::pair<std::string, std::string>("Allow", methods));
}


#include <iostream>
static int	check_body_size_validity(const Location_block &location, const Request &request, Response &response)
{
	const std::map<std::string, std::string>::const_iterator	content_length_iterator = request.headers.find("Content-Length");

	if (request.method == "POST" && content_length_iterator == request.headers.end())
		return (set_status_code_and_return(response, "405")); // Should be 411 ...
	if (content_length_iterator != request.headers.end())
	{
		if (atoll(content_length_iterator->second.c_str()) > location.client_max_body_size)
			return (set_status_code_and_return(response, "413"));
	}
	if (static_cast<long long int>(request.body.size()) > location.client_max_body_size)
		return (set_status_code_and_return(response, "413"));
	return (0);
}

static int	check_method_validity(const Location_block &location, const Request &request, Response &response)
{
	if (std::find(location.limit_except.begin(), location.limit_except.end(), request.method) == location.limit_except.end())
	{
		add_allow_header(location, response);
		return (set_status_code_and_return(response, "405"));
	}
	return (0);
}

int	post_parsing(const Location_block &location, const Request &request, Response &response)
{
	if (check_body_size_validity(location, request, response) < 0)
		return (-1);
	if (check_method_validity(location, request, response) < 0)
		return (-1);
	return (0);
}

int	set_status_code_and_return(Response &response, const std::string &status_code)
{
	response.status_code = status_code;
	return (-1);
}
