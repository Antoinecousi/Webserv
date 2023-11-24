/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 19:30:07 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/03 16:34:45 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <cstdlib>
#include <cerrno>
#include <ctime>
#include <dirent.h>
#include <sys/stat.h>
#include "Location_block.hpp"
#include "request.hpp"
#include "response.hpp"
#include "parsing.hpp"

static int	get_file(Response &response, const std::string &path)
{
	std::ifstream			ifs;
	std::ostringstream		oss;

	ifs.open(path.c_str());
    if (!(ifs.is_open()))
		return (set_status_code_and_return(response, "500"));
	oss << ifs.rdbuf();
	if (ifs.fail())
	{
		ifs.close();
		return (set_status_code_and_return(response, "500"));
	}
	response.body = oss.str();
	ifs.close();
	return (0);
}

static std::string	get_last_modified_date_gmt(time_t raw_time)
{
	struct tm	*gmt;
    char		buffer[32];

	gmt = std::gmtime(&raw_time);
    if (std::strftime(buffer, 32, "%a, %d %b %Y %H:%M:%S %Z", gmt) == 0)
		return (std::string());
    return (std::string(buffer));
}

static std::string	get_mime_type(const Response& response, const std::string &filename)
{
	std::map<std::string, std::string>::const_iterator	it;
	std::string::size_type								pos;

	pos = filename.find('.');
	if (pos == std::string::npos)
		return (std::string("text/plain"));
	else
		pos++;
	it = response.mime_types.find(filename.substr(pos));
	while (it == response.mime_types.end())
	{
		pos = filename.find('.', pos);
		if (pos == std::string::npos)
			return (std::string("text/plain"));
		else
			pos++;
		it = response.mime_types.find(filename.substr(pos));
	}
	return (it->second);
}

static int	isdir_filter(const struct dirent *entry)
{
	return (entry->d_type == DT_DIR);
}

static int	isreg_filter(const struct dirent *entry)
{
	return (entry->d_type == DT_REG);
}

static std::string	get_last_modified_date_local(time_t raw_time)
{
    const char	months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	struct tm	*local;
    char		buffer[32];

	local = std::localtime(&raw_time);
    std::sprintf(buffer, "%02d-%s-%04d %02d:%02d", local->tm_mday, months[local->tm_mon], local->tm_year + 1900, local->tm_hour, local->tm_min);
    return (std::string(buffer));
}

static int	autoindex(Response &response, const std::string &path, const std::string &uri)
{
	std::ostringstream	oss;
	struct dirent		**entries;
	struct stat			file_info;
	std::string			filename;
	std::string			href;
	int					n;

	oss << "<html>\r\n";
	oss << "<head><title>Index of " << uri << "</title></head>\r\n";
	oss << "<body>\r\n";
	oss << "<h1>Index of " << uri << "</h1><hr><pre><a href=\"../\">../</a>\r\n";

	n = scandir(path.c_str(), &entries, isdir_filter, alphasort);
	if (n < 0)
		return (set_status_code_and_return(response, "500"));
	for (int i = 0; i < n; i++)
	{
		if (entries[i]->d_type == DT_REG || entries[i]->d_type == DT_DIR)
		{
			filename = std::string(entries[i]->d_name) + '/';
			if (filename[0] == '.')
			{
				free(entries[i]);
				continue ;
			}
			if (stat((path + '/' + filename).c_str(), &file_info) == 0)
			{
				if (filename.size() == 51)
					filename = filename.substr(0, 50);
				else if (filename.size() > 51)
					filename = filename.substr(0, 47) + "..&gt;";
				if (uri == "/")
					href = entries[i]->d_name;
				else
					href = uri + "/" + entries[i]->d_name;
				// Width of 54 (50 for the filename and 4 for "</a>")
				oss << "<a href=\"" << href << "\">" << std::setw(54) << std::left << filename + "</a>" << " " << get_last_modified_date_local(file_info.st_mtime) << std::setw(19) << std::right << "-" << "\r\n";
			}
		}
		free(entries[i]);
	}
	free(entries);

	n = scandir(path.c_str(), &entries, isreg_filter, alphasort);
	if (n < 0)
		return (set_status_code_and_return(response, "500"));
	for (int i = 0; i < n; i++)
	{
		if (entries[i]->d_type == DT_REG || entries[i]->d_type == DT_DIR)
		{
			filename = std::string(entries[i]->d_name);
			if (filename[0] == '.')
			{
				free(entries[i]);
				continue ;
			}
			if (stat((path + '/' + filename).c_str(), &file_info) == 0)
			{
				if (filename.size() > 50)
					filename = filename.substr(0, 47) + "..&gt;";
				if (uri == "/")
					href = entries[i]->d_name;
				else
					href = uri + "/" + entries[i]->d_name;
				// Width of 54 (50 for the filename and 4 for "</a>")
				oss << "<a href=\"" << href << "\">" << std::setw(54) << std::left << filename + "</a>" << " " << get_last_modified_date_local(file_info.st_mtime) << std::setw(19) << std::right << file_info.st_size << "\r\n";
			}
		}
		free(entries[i]);
	}
	free(entries);

	oss << "</pre><hr></body>\r\n";
	oss << "</html>\r\n";
	response.body = oss.str();
	return (0);
}

int	get(const Location_block &location, const Request &request, Response &response)
{
	struct stat													file_info;
	struct stat													subfile_info;
	std::string													path;
	std::string													last_modified_date;

	// Check the validity of the body size and the method
	if (post_parsing(location, request, response) < 0)
		return (-1);

	// Construct the path
	path = location.root + request.uri;
	if (stat(path.c_str(), &file_info) < 0)
	{
		// The path is invalid
		if (errno == EACCES)
			return (set_status_code_and_return(response, "403"));
		if (errno == ENOENT || errno == ENOTDIR)
			return (set_status_code_and_return(response, "404"));
		if (errno == ELOOP || errno == ENAMETOOLONG)
			return (set_status_code_and_return(response, "414"));
		else
			return (set_status_code_and_return(response, "500"));
	}
	if (S_ISREG(file_info.st_mode))
	{
		// The path points to a file
		if (get_file(response, path) < 0)
			return (-1);
		// Get the last modification date of the file in gmt date format
		last_modified_date = get_last_modified_date_gmt(file_info.st_mtime);
		if (!(last_modified_date.empty()))
			response.headers.push_front(std::pair<std::string, std::string>("Last-Modified", last_modified_date));
		// Add the other response headers
		response.headers.push_front(std::pair<std::string, std::string>("Content-Length", INT_TO_STRING(response.body.size())));
		response.headers.push_front(std::pair<std::string, std::string>("Content-Type", get_mime_type(response, path.substr(path.rfind('/') + 1))));
		response.status_code = "200";
		return (0);
	}
	else if (S_ISDIR(file_info.st_mode))
	{
		// The path points to a directory. Three cases must be handled in this order.
		//     1. Use a valid index file.
		//     2. Display the contents of the directory (autoindex)
		//     3. Error 404 (Not Found)

		// Loop on all indexes
		for (std::vector<std::string>::const_iterator it = location.index.begin(); it != location.index.end(); it++)
		{
			if (stat((path + '/' + *it).c_str(), &subfile_info) < 0)
			{
				if (errno == EACCES)
					return (set_status_code_and_return(response, "403"));
				if (errno == ENOENT)
					continue ;
				if (errno == ELOOP || errno == ENAMETOOLONG)
					return (set_status_code_and_return(response, "414"));
				else
					return (set_status_code_and_return(response, "500"));
			}
			// The current index is valid
			if (get_file(response, path + '/' + *it) < 0)
				return (-1);
			// Get the last modification date of the file in gmt date format
			last_modified_date = get_last_modified_date_gmt(file_info.st_mtime);
			if (!(last_modified_date.empty()))
				response.headers.push_front(std::pair<std::string, std::string>("Last-Modified", last_modified_date));
			// Add the other response headers
			response.headers.push_front(std::pair<std::string, std::string>("Content-Length", INT_TO_STRING(response.body.size())));
			response.headers.push_front(std::pair<std::string, std::string>("Content-Type", get_mime_type(response, (*it).substr((*it).rfind('/') + 1))));
			response.status_code = "200";
			return (0);
		}
		// There is no valid index
		if (location.autoindex)
		{
			// Display the contents of the directory (autoindex)
			if (autoindex(response, path, request.uri) < 0)
				return (-1);
			response.headers.push_front(std::pair<std::string, std::string>("Content-Type", "text/html"));
			response.status_code = "200";
			return (0);
		}
		else
		{
			// No index was found and the autoindex is disabled.
			return (set_status_code_and_return(response, "404"));
		}
	}
	else
	{
		// The path points to another type of file.
		return (set_status_code_and_return(response, "403"));
	}
}
