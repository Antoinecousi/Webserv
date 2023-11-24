/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 06:39:46 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/04 10:53:17 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

/* ************************************************************************** */
/*                                 Libraries                                  */
/* ************************************************************************** */

# include <string>
# include <sstream>
# include <map>
# include "Http_block.hpp"
# include "response.hpp"

/* ************************************************************************** */
/*                                 Structures                                 */
/* ************************************************************************** */

class Response;
class Request
{
	public:

	std::string							host;
	std::string							port;
	std::string							method;
	std::string							uri;
	std::string							query;
	std::string							version;
	std::map<std::string, std::string>	headers;
	std::string							body;
};

/* ************************************************************************** */
/*                           Function declarations                            */
/* ************************************************************************** */

int	http_request_parse_core(Request &request, Response &response, std::istringstream &iss);
int	http_request_parse_line(Request &request, Response &response, std::string &line);
int	http_request_parse_headers(Request &request, Response &response, std::string &line);
int	process_request(const Http_block &http, const std::string &host, const std::string &port, const std::string &str_request, std::string &str_response, bool & keep_alive);


#endif
