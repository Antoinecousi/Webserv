/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   listen.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:37:19 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:09:25 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include <utility>
#include <string>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include "Server_block.hpp"
#include "parsing.hpp"

static std::string	resolve_hostname(const std::string &directive, const std::string::size_type l, const std::string &host_port, const std::string &hostname)
{
	char			ip[INET_ADDRSTRLEN];
	struct addrinfo	hints;
	struct addrinfo	*result;

	// Set up hints structure (IPV4)
	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;

	// Call getaddrinfo
	if (getaddrinfo(hostname.c_str(), NULL, &hints, &result) != 0)
		return (host_not_found_error(directive, l, host_port));

	// Extract the IP address from the result
	if (!(inet_ntop(AF_INET, &(reinterpret_cast<struct sockaddr_in*>(result->ai_addr)->sin_addr), ip, INET_ADDRSTRLEN)))
	{
		freeaddrinfo(result);
		return (host_not_found_error(directive, l, host_port));
	}

	// Free the result of getaddrinfo
	freeaddrinfo(result);
	return (std::string(ip));
}

int	parse_listen(std::pair<e_context, void*> &context, const std::string &directive, const std::vector<std::string> &args, const std::string::size_type l)
{
	std::string							host;
	std::string							port;
	std::vector<std::string>			host_components;
	std::string::size_type				start;
	std::string::size_type				end;

	// Split listen into host and port
	start = args[0].find(':');
	end = args[0].rfind(':');
	if (start != end)
		return (invalid_value_error(directive, l, args[0]));
	if (start == std::string::npos)
	{
		if (args[0] == "*")
		{
			host = "0.0.0.0";
			port = !(geteuid()) ? "80" : "8000";
		}
		else if (args[0].find_first_not_of("0123456789.") != std::string::npos)
		{
			host = resolve_hostname(directive, l, args[0], args[0]);
			if (host.empty())
				return (-1);
			port = !(geteuid()) ? "80" : "8000";
		}
		else if (args[0].find('.') == std::string::npos)
		{
			host = "0.0.0.0";
			port = args[0];
		}
		else
		{
			host = args[0];
			port = !(geteuid()) ? "80" : "8000";
		}
	}
	else
	{
		host = args[0].substr(0, end);
		port = args[0].substr(start + 1);
		if (host == "*")
		{
			host = "0.0.0.0";
		}
		else if (host.find_first_not_of("0123456789.") != std::string::npos)
		{
			host = resolve_hostname(directive, l, args[0], host);
			if (host.empty())
				return (-1);
		}
	}

	// Split host into multiple host_components using '.' as delimiter
	start = 0;
	end = host.find('.');
	while (end != std::string::npos)
	{
		if (start == end)
			return (invalid_host_error(directive, l, args[0]));
		host_components.push_back(host.substr(start, end - start));
		start = end + 1;
		end = host.find('.', start);
	}
	if (start == end)
		return (invalid_host_error(directive, l, args[0]));
	if (start < host.size())
		host_components.push_back(host.substr(start));
	if (host_components.size() != 4)
		return (invalid_host_error(directive, l, args[0]));

	for (std::vector<std::string>::const_iterator it = host_components.begin(); it != host_components.end(); it++)
	{
		// Check host validity
		if ((*it).find_first_not_of("0123456789") != std::string::npos)
			return (invalid_host_error(directive, l, args[0]));
		// Check host limit
		if ((*it).size() > 3 || ((*it).size() == 3 && (*it).compare("255") > 0))
			return (host_must_be_between_error(directive, l, host, 0, 255));
	}

	// Check port validity
	if (port.find_first_not_of("0123456789") != std::string::npos)
		return (invalid_port_error(directive, l, args[0]));
	// Check port limit
	if (port.size() > 5 || (port.size() == 5 && port.compare("65535") > 0))
		return (port_must_be_between_error(directive, l, port, 0, 65535));

	Server_block	&server = get_context<Server_block>(context);
	for (std::vector< std::pair<std::string, std::string> >::const_iterator it = server.listen.begin(); it != server.listen.end(); it++)
	{
		if ((*it).first == host && (*it).second == port)
			return (duplicate_listening_error(directive, l, host, port));
	}
	server.listen.push_back(std::pair<std::string, std::string>(host, port));
	server.set_flag_listen(true);
	return (0);
}
