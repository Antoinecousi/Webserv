/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/01 16:46:31 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/03 16:33:36 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <cstdio>
#include <cerrno>
#include <dirent.h>
#include <sys/stat.h>
#include "Location_block.hpp"
#include "request.hpp"
#include "response.hpp"
#include "parsing.hpp"

static int	delete_file(Response &response, const std::string &path)
{
	if (remove(path.c_str()) < 0)
		return (set_status_code_and_return(response, "500"));
	return (0);
}

int	delete_(const Location_block &location, const Request &request, Response &response)
{
	struct stat	file_info;
	std::string	path;

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
		if (delete_file(response, path) < 0)
			return (-1);
		// Add the other response headers
		response.status_code = "204";
		return (0);
	}
	else
	{
		// The path points to another type of file.
		return (set_status_code_and_return(response, "403"));
	}
}
