/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   post.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 19:19:44 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/04 08:26:37 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <cstring>
#include <climits>
#include <cerrno>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "Location_block.hpp"
#include "request.hpp"
#include "response.hpp"
#include "parsing.hpp"

static int	generate_filepath(const Location_block &location, const Request &request, Response &response, std::string &path, std::string &filename)
{
	std::string				try_path;
	std::string				basename;
	std::string				extension;
	struct stat				subfile_info;
	unsigned long long int	i;

	if (filename.empty())
		filename = "untitled";
	try_path = location.root + request.uri + '/' + filename;
	if (!(filename.empty()) && stat(try_path.c_str(), &subfile_info) < 0)
	{
		if (errno != ENOENT)
			return (set_status_code_and_return(response, "500"));
	}
	else
	{
		if (filename.find('.') != std::string::npos)
		{
			basename = filename.substr(0, filename.find('.'));
			extension = filename.substr(filename.find('.'));
		}
		else
		{
			basename = filename;
			extension = std::string();
		}

		i = 1;
		while (i < ULLONG_MAX)
		{
			filename = basename + " (" + INT_TO_STRING(i) + ")" + extension;
			try_path = location.root + request.uri + '/' + filename;
			if (stat(try_path.c_str(), &subfile_info) < 0)
			{
				if (errno == ENOENT)
					break ;
				else
					return (set_status_code_and_return(response, "500"));
			}
			i++;
		}
	}
	path = try_path;
	return (0);
}

static int	create_file(Response &response, const std::string &path, std::string &body)
{
	std::ofstream	ofs;

	ofs.open(path.c_str(), std::ofstream::trunc);
    if (!(ofs.is_open()))
		return (set_status_code_and_return(response, "500"));

	ofs << body;
	if (ofs.fail())
	{
		ofs.close();
		return (set_status_code_and_return(response, "500"));
	}
	ofs.close();
	return (0);
}

static std::string	resolve_ip_address(const std::string &ip_address)
{
	char			hostname[NI_MAXHOST];
	struct addrinfo	hints;
	struct addrinfo	*result;

	// Set up hints structure (IPV4)
	std::memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;

	// Call getaddrinfo
	if (getaddrinfo(ip_address.c_str(), NULL, &hints, &result) != 0)
		return (ip_address);

	// Extract the IP address from the result
	if (getnameinfo(result->ai_addr, result->ai_addrlen, hostname, NI_MAXHOST, NULL, 0, 0) != 0)
	{
		freeaddrinfo(result);
		return (ip_address);
	}

	// Free the result of getaddrinfo
	freeaddrinfo(result);
	return (std::string(hostname));
}

static void	encode_uri(std::string &uri)
{
	const std::string::size_type	size = uri.size();
	std::string						encoded_uri;
	char							buffer[4];

	for (std::string::size_type i = 0; i < size; i++)
	{
		if (uri[i] < ' ' || '~' < uri[i] \
		||  uri[i] == 0x20 \
		||  uri[i] == 0x22 \
		||  uri[i] == 0x23 \
		||  uri[i] == 0x25 \
		||  uri[i] == 0x3C \
		||  uri[i] == 0x3E \
		||  uri[i] == 0x5B \
		||  uri[i] == 0x5C \
		||  uri[i] == 0x5D \
		||  uri[i] == 0x5E \
		||  uri[i] == 0x60 \
		||  uri[i] == 0x7B \
		||  uri[i] == 0x7C \
		||  uri[i] == 0x7D)
		{
			std::memset(buffer, 0, sizeof(char [4]));
			sprintf(buffer, "%%%02X", uri[i]);
			encoded_uri.append(buffer);
		}
		else
		{
			encoded_uri.push_back(uri[i]);
		}
	}

	uri = encoded_uri;
}

int	post(const Location_block &location, Request &request, Response &response)
{
	struct stat				file_info;
	std::string				path;
	std::istringstream		iss;
	std::string				line;
	std::string				previous_line;
	bool					flag_previous_line;
	std::string				boundary;
	std::string				filename;
	size_t					nb_files;
	std::string				location_header;
	std::string				body;
	std::string::size_type	begin;
	std::string::size_type	end;

	// Check the validity of the body size and the method
	if (post_parsing(location, request, response) < 0)
		return (-1);

	// Construct the path to the upload folder
	path = location.root + request.uri;
	if (stat(path.c_str(), &file_info) < 0)
	{
		if (errno == EACCES)
			return (set_status_code_and_return(response, "403"));
		if (errno == ENOENT || errno == ENOTDIR)
			return (set_status_code_and_return(response, "404"));
		if (errno == ELOOP || errno == ENAMETOOLONG)
			return (set_status_code_and_return(response, "414"));
		else
			return (set_status_code_and_return(response, "500"));
	}
	if (!(S_ISDIR(file_info.st_mode)))
		return (set_status_code_and_return(response, "403"));

	begin = request.headers["Content-Type"].find("multipart/") + 10;
	if (begin == std::string::npos + 10)
	{
		// Generate filepath
		if (generate_filepath(location, request, response, path, filename) < 0)
			return (-1);
		// Write to the file
		if (create_file(response, path, request.body) < 0)
			return (-1);

		// Construct the URL for the Location header
		location_header = "http://" + resolve_ip_address(request.host) + ":" + request.port + request.uri + '/' + filename;
	}
	else
	{
		// Get the separation string
		begin = request.headers["Content-Type"].find(";", begin) + 1;
		if (begin == std::string::npos + 1)
			return (set_status_code_and_return(response, "400"));
		begin = request.headers["Content-Type"].find("boundary=", begin) + 9;
		end = request.headers["Content-Type"].find_first_of(" ", begin);
		if (begin == std::string::npos + 9)
			return (set_status_code_and_return(response, "400"));
		if (end != std::string::npos)
			boundary = request.headers["Content-Type"].substr(begin, end - begin);
		else
			boundary = request.headers["Content-Type"].substr(begin);

		// Reading the body
		iss.str(request.body);
	    while (std::getline(iss, line))
		{
			// Skipping additional CRLFs that may precede the first boundary string in the entity
			if (line.empty() || (line.size() == 1 && line[0] == '\r'))
				continue ;
			else
				break ;
	    }
		nb_files = 0;
		while (iss.good())
		{
			// Multipart - End delimiter
			if (line == "--" + boundary + "--" || line == "--" + boundary + "--" + '\r')
				break ;
			// Range - Start - delimiter
			if (line != "--" + boundary && line != "--" + boundary + '\r' )
				return (set_status_code_and_return(response, "400"));

			nb_files++;
			// Range - headers
			filename = std::string();
			while (std::getline(iss, line))
			{
				// Content-Disposition
				begin = line.find("Content-Disposition:") + 20;
				if (begin == 20)
				{
					begin = line.find("filename=\"", begin) + 10;
					if (begin == std::string::npos + 10)
						continue ;
					end = line.find("\"", begin);
					if (end == std::string::npos)
						return (set_status_code_and_return(response, "400"));
					filename = line.substr(begin, end - begin);
				}
				// Break on the first line containing only a CLRF
				if (line.empty() || (line.size() == 1 && line[0] == '\r'))
					break ;
			}

			// Range - body
			body = std::string();
			flag_previous_line = false;
			previous_line = std::string();
			while (std::getline(iss, line))
			{
				// Break if the line is the seperation string or the end string
				if (line == "--" + boundary || line == "--" + boundary + '\r' \
				||  line == "--" + boundary + "--" || line == "--" + boundary + "--" + '\r')
					break ;
				// Shift of one line to avoid writing an empty line at the end of
				// the file which is in fact an additional CLRF added by the client
				// (which is not part of the rfc standard but is frequent)
				if (!(flag_previous_line))
				{
					previous_line = line;
					flag_previous_line = true;
					continue ;
				}
				else
				{
					body += previous_line + '\n';
				}
				previous_line = line;
			}
			if (!(previous_line.empty()))
				body += previous_line + '\n';
			if (iss.fail())
				break ;

			// Deletion of the last two characters (\r\n) of the file added by the client
			if (body.size() >= 2 && body[body.size() - 2] == '\r' && body[body.size() - 1] == '\n')
				body.erase(body.size() - 2, 2);

			// Generate filepath
			if (generate_filepath(location, request, response, path, filename) < 0)
				return (-1);
			// Write to the file
			if (create_file(response, path, body) < 0)
				return (-1);
		}
		if (iss.fail())
			return (set_status_code_and_return(response, "500"));

		// Construct the URL for the Location header
		location_header = "http://" + resolve_ip_address(request.host) + ":" + request.port + request.uri + '/';
		if (nb_files == 0)
			return (set_status_code_and_return(response, "204"));
		else if (nb_files == 1)
			location_header += filename;
		else
			location_header += '*';
	}
	encode_uri(location_header);
	response.headers.push_back(std::pair<std::string, std::string>("Location", location_header));
	response.status_code = "201";
	return (0);
}
