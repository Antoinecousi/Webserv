/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generate_response.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 19:11:37 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/04 15:01:55 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <map>
#include <utility>
#include <ctime>
#include "Location_block.hpp"
#include "response.hpp"
#include "parsing.hpp"

static std::string	get_reason_phrase(const Response& response)
{
	std::map<std::string, std::string>::const_iterator	it = response.status_code_definitions.find(response.status_code);

	if (it == response.status_code_definitions.end())
		return (std::string());
	return (it->second);
}

static std::string	get_current_date_gmt(void)
{
	time_t		raw_time;
	struct tm	*gmt;
    char		buffer[32];

	if (std::time(&raw_time) < 0)
		return (std::string());
	gmt = std::gmtime(&raw_time);
    if (std::strftime(buffer, 32, "%a, %d %b %Y %H:%M:%S %Z", gmt) == 0)
		return (std::string());
    return (buffer);
}

static std::string	get_hardcoded_error_page(const std::string &status_code, const std::string &reason_phrase)
{
	std::string	body;

	body += "<html>\r\n";
	body += "<head><title>" + status_code + " " + reason_phrase + "</title></head>\r\n";
	body += "<body>\r\n";
	body += "<center><h1>" + status_code + " " + reason_phrase + "</h1></center>\r\n";
	body += "<hr><center>webserv</center>\r\n";
	body += "</body>\r\n";
	body += "</html>\r\n";
	return (body);
}

void	generate_response(const Location_block &location, Response &response, bool request_fail)
{
	const std::map<int, std::string>::const_iterator	it = location.error_page.find(atoi(response.status_code.c_str()));
	const std::string									date = get_current_date_gmt();

	response.version = "HTTP/1.1";
	response.reason_phrase = get_reason_phrase(response);

	if (!(request_fail) && location.return_.first)
	{
		response.reason_phrase = get_reason_phrase(response);
		if (location.return_.first == 301 \
		||  location.return_.first == 302 \
		||  location.return_.first == 303 \
		||  location.return_.first == 307)
		{
			response.headers.push_back(std::pair<std::string, std::string>("Location", location.return_.second));
			response.body = get_hardcoded_error_page(response.status_code, response.reason_phrase);
			response.headers.push_front(std::pair<std::string, std::string>("Content-Length", INT_TO_STRING(response.body.size())));
			response.headers.push_front(std::pair<std::string, std::string>("Content-Type", "text/html"));
		}
		else
		{
			response.body = location.return_.second;
			response.headers.push_front(std::pair<std::string, std::string>("Content-Length", INT_TO_STRING(response.body.size())));
			response.headers.push_front(std::pair<std::string, std::string>("Content-Type", "text/plain"));
		}
	}
	else if ("400" <= response.status_code && response.status_code <= "599")
	{
		if (it != location.error_page.end())
		{
			response.status_code = "302";
			response.reason_phrase = get_reason_phrase(response);
			response.headers.push_back(std::pair<std::string, std::string>("Location", it->second));
		}
		response.body = get_hardcoded_error_page(response.status_code, response.reason_phrase);
		response.headers.push_front(std::pair<std::string, std::string>("Content-Length", INT_TO_STRING(response.body.size())));
		response.headers.push_front(std::pair<std::string, std::string>("Content-Type", "text/html"));
	}
	if (!(date.empty()))
		response.headers.push_front(std::pair<std::string, std::string>("Date", date));
	response.headers.push_front(std::pair<std::string, std::string>("Server", "webserv"));
}
