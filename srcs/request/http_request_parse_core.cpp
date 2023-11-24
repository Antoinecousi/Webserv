/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   http_request_parse_core.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 06:39:46 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/04 16:12:45 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include <cctype>
#include "request.hpp"
#include "response.hpp"

int	http_request_parse_core(Request &request, Response &response, std::istringstream &iss)
{
    std::string	line;

    while (std::getline(iss, line))
	{
		if (!(line.empty() || (line.size() == 1 && line[0] == '\r')))
			break ;
	}
	if (http_request_parse_line(request, response, line) < 0)
		return (-1);
    while (std::getline(iss, line))
	{
		if (line.empty() || (line.size() == 1 && line[0] == '\r'))
			break ;
		if (http_request_parse_headers(request, response, line) < 0)
			return (-1);
	}
	if (!(iss.eof()))
		request.body = iss.str().substr(iss.tellg());
	iss.seekg(0, iss.end);
	if (!(request.headers.count("Host")))
		return (set_status_code_and_return(response, "400"));
	return (0);
}
