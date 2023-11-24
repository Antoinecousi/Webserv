/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_request.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 19:08:47 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/04 14:59:41 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <sstream>
#include "Http_block.hpp"
#include "Server_block.hpp"
#include "Location_block.hpp"
#include "request.hpp"
#include "response.hpp"
#include "server.hpp"
#include "parsing.hpp"

static const Location_block	&default_server_location(const Http_block &http, const Request &request)
{
	for (std::vector<Server_block>::const_iterator server_it = http.server.begin(); server_it != http.server.end(); server_it++)
	{
		for (std::vector< std::pair<std::string, std::string> >::const_iterator server_listen_it = (*server_it).listen.begin(); server_listen_it != (*server_it).listen.end(); server_listen_it++)
		{
			if ((*server_listen_it).first == request.host && (*server_listen_it).second == request.port)
			{
				for (std::vector<Location_block>::const_iterator location_it = (*server_it).location.begin(); location_it != (*server_it).location.end(); location_it++)
				{
					if ((*location_it).uri == "/" && (*location_it).equal_modifier == false)
					{
						return (*location_it);
					}
				}
			}
		}
	}
	return (http.server[0].location[0]);
}

int	process_request(const Http_block &http, const std::string &host, const std::string &port, const std::string &str_request, std::string &str_response, bool & keep_alive)
{
	Request				request;
	Response			response;
	std::istringstream	iss;

	request.host = host;
	request.port = port;
	iss.str(str_request);
	if (http_request_parse_core(request, response, iss) == 0)
	{
		const Location_block	&location = pickLocation(http, request);

		if (request.headers.count("Connection") && request.headers.find("Connection")->second != "close")
		{
			response.headers.push_front(std::pair<std::string, std::string>("Connection", "keep-alive"));
			keep_alive = true;
		}
		else
		{
			response.headers.push_front(std::pair<std::string, std::string>("Connection", "close"));
			keep_alive = false;
		}

		if (location.return_.first)
		{
			response.status_code = INT_TO_STRING(location.return_.first);
		}
		else
		{
			if (is_CGI_php(request) || is_CGI_python(request))
			{
				if (request.method == "POST")
					request.query = request.body;
				process_by_CGI(location, request, response);
			}
			else if (request.method == "GET")
				get(location, request, response);
			else if (request.method == "POST")
				post(location, request, response);
			else if (request.method == "DELETE")
				delete_(location, request, response);
			else
				response.status_code = "501";
		}

		generate_response(location, response);
	}
	else
	{
		const Location_block &location = default_server_location(http, request);

		generate_response(location, response, true);
	}

	str_response = response.version + " " + response.status_code + " " + response.reason_phrase + "\r\n";
	for (std::list< std::pair<std::string, std::string> >::const_iterator it = response.headers.begin(); it != response.headers.end(); it++)
		str_response += it->first + ": " + it->second + "\r\n";
	if (!is_CGI_php(request))
		str_response += "\r\n";
	str_response += response.body;
	return (0);
}
