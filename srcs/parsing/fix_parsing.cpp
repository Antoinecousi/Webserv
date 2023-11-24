/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fix_parsing.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acousini <acousini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:32:46 by spayeur           #+#    #+#             */
/*   Updated: 2023/11/24 15:12:02 by acousini         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <pwd.h>
#include <iostream>
#include <cstring>
#include "Http_block.hpp"
#include "Server_block.hpp"
#include "Location_block.hpp"

static void	undiff(const Http_block &http, Server_block &server)
{
	if (http.get_flag_error_page() && !(server.get_flag_error_page()))
	{
		server.error_page = http.error_page;
		server.set_flag_error_page(true);
	}
	if (http.get_flag_client_max_body_size() && !(server.get_flag_client_max_body_size()))
	{
		server.client_max_body_size = http.client_max_body_size;
		server.set_flag_client_max_body_size(true);
	}
	if (http.get_flag_root() && !(server.get_flag_root()))
	{
		server.root = http.root;
		server.set_flag_root(true);
	}
	if (http.get_flag_autoindex() && !(server.get_flag_autoindex()))
	{
		server.autoindex = http.autoindex;
		server.set_flag_autoindex(true);
	}
	if (http.get_flag_index() && !(server.get_flag_index()))
	{
		server.index = http.index;
		server.set_flag_index(true);
	}
}

static void	undiff(const Server_block &server, Location_block &location)
{
	if (server.get_flag_error_page() && !(location.get_flag_error_page()))
	{
		location.error_page = server.error_page;
		location.set_flag_error_page(true);
	}
	if (server.get_flag_client_max_body_size() && !(location.get_flag_client_max_body_size()))
	{
		location.client_max_body_size = server.client_max_body_size;
		location.set_flag_client_max_body_size(true);
	}
	if (server.get_flag_return_() && !(location.get_flag_return_()))
	{
		location.return_ = server.return_;
		location.set_flag_return_(true);
	}
	if (server.get_flag_root() && !(location.get_flag_root()))
	{
		location.root = server.root;
		location.set_flag_root(true);
	}
	if (server.get_flag_autoindex() && !(location.get_flag_autoindex()))
	{
		location.autoindex = server.autoindex;
		location.set_flag_autoindex(true);
	}
	if (server.get_flag_index() && !(location.get_flag_index()))
	{
		location.index = server.index;
		location.set_flag_index(true);
	}
}

static void	undiff(const Location_block &location, Location_block &sublocation)
{
	if (location.get_flag_error_page() && !(sublocation.get_flag_error_page()))
	{
		sublocation.error_page = location.error_page;
		sublocation.set_flag_error_page(true);
	}
	if (location.get_flag_client_max_body_size() && !(sublocation.get_flag_client_max_body_size()))
	{
		sublocation.client_max_body_size = location.client_max_body_size;
		sublocation.set_flag_client_max_body_size(true);
	}
	if (location.get_flag_limit_except() && !(sublocation.get_flag_limit_except()))
	{
		sublocation.limit_except = location.limit_except;
		sublocation.set_flag_limit_except(true);
	}
	if (location.get_flag_return_() && !(sublocation.get_flag_return_()))
	{
		sublocation.return_ = location.return_;
		sublocation.set_flag_return_(true);
	}
	if (location.get_flag_root() && !(sublocation.get_flag_root()))
	{
		sublocation.root = location.root;
		sublocation.set_flag_root(true);
	}
	if (location.get_flag_autoindex() && !(sublocation.get_flag_autoindex()))
	{
		sublocation.autoindex = location.autoindex;
		sublocation.set_flag_autoindex(true);
	}
	if (location.get_flag_index() && !(sublocation.get_flag_index()))
	{
		sublocation.index = location.index;
		sublocation.set_flag_index(true);
	}
	if (location.get_flag_cgi() && !(sublocation.get_flag_cgi()))
	{
		sublocation.cgi = location.cgi;
		sublocation.set_flag_cgi(true);
	}
}

static std::string get_default_root(void)
{
    char exePath[4096];
    ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);

    if (len != -1)
    {
        exePath[len] = '\0';
        char* lastSlash = std::strrchr(exePath, '/');
        
        if (lastSlash != NULL)
        {
            *lastSlash = '\0';
			// std::cout << "Path for html execution files: " << std::string(exePath) + "/html" << std::endl;
            return std::string(exePath) + "/html";
        }
    }
	const struct passwd	*pws = getpwuid(geteuid());
	const std::string	username = !(pws) ? "root" : pws->pw_name;
	std::cout << "the program couldn't access the path, path to default is now : " << "/goinfre/" + username + "/webserv/html" << std::endl;
	return ("/goinfre/" + username + "/webserv/html");
}

static void	set_default(Http_block &http)
{
	if (!(http.get_flag_error_page()))
		http.error_page = std::map<int, std::string>();
	if (!(http.get_flag_client_max_body_size()))
		http.client_max_body_size = 1048576; // equals 1 * 1024 * 1024 or 1MiB (1m)
	if (!(http.get_flag_server()))
		http.server = std::vector<Server_block>();
	if (!(http.get_flag_root()))
		http.root = get_default_root();
	if (!(http.get_flag_autoindex()))
		http.autoindex = false;
	if (!(http.get_flag_index()))
		http.index = std::vector<std::string>(1, "index.html");
}

static void	set_default(Server_block &server)
{
	bool	found;

	if (!(server.get_flag_listen()))
		server.listen = std::vector< std::pair<std::string, std::string> >(1, std::pair<std::string, std::string>("0.0.0.0", (!(geteuid()) ? "80" : "8000")));
	if (!(server.get_flag_server_name()))
		server.server_name = std::vector<std::string>();
	if (!(server.get_flag_error_page()))
		server.error_page = std::map<int, std::string>();
	if (!(server.get_flag_client_max_body_size()))
		server.client_max_body_size = 1048576; // equals 1 * 1024 * 1024 or 1MiB (1m)
	if (!(server.get_flag_location()))
		server.location = std::vector<Location_block>(1, Location_block("/", false));
	else
	{
		// Define a default location / in case no location / is defined
		found = false;
		for (std::vector<Location_block>::const_iterator it = server.location.begin(); it != server.location.end(); it++)
		{
			if ((*it).uri == "/" && (*it).equal_modifier == false)
			{
				found = true;
				break ;
			}
		}
		if (!(found))
			server.location.push_back(Location_block("/", false));
	}
	if (!(server.get_flag_return_()))
		server.return_ = std::pair<int, std::string>(int(), std::string());
	if (!(server.get_flag_root()))
		server.root = get_default_root();
	if (!(server.get_flag_autoindex()))
		server.autoindex = false;
	if (!(server.get_flag_index()))
		server.index = std::vector<std::string>(1, "index.html");
}

static void	set_default(Location_block &location)
{
	if (!(location.get_flag_error_page()))
		location.error_page = std::map<int, std::string>();
	if (!(location.get_flag_client_max_body_size()))
		location.client_max_body_size = 1048576; // equals 1 * 1024 * 1024 or 1MiB (1m)
	if (!(location.get_flag_location()))
		location.location = std::vector<Location_block>();
	if (!(location.get_flag_limit_except()))
	{
		location.limit_except = std::vector<std::string>();
		location.limit_except.push_back("GET");
		location.limit_except.push_back("POST");
		location.limit_except.push_back("DELETE");
	}
	if (!(location.get_flag_return_()))
		location.return_ = std::pair<int, std::string>(int(), std::string());
	if (!(location.get_flag_root()))
		location.root = get_default_root();
	if (!(location.get_flag_autoindex()))
		location.autoindex = false;
	if (!(location.get_flag_index()))
		location.index = std::vector<std::string>(1, "index.html");
	if (!(location.get_flag_cgi()))
		location.cgi = std::map<std::string, std::string>();
}

static void	fix_parsing_location(Location_block &location)
{
	set_default(location);
	for (std::vector<Location_block>::iterator sublocation_it = location.location.begin(); sublocation_it != location.location.end(); sublocation_it++)
	{
		undiff(location, *sublocation_it);
		fix_parsing_location(*sublocation_it);
	}
}

void	fix_parsing(Http_block &http)
{
	set_default(http);
	for (std::vector<Server_block>::iterator server_it = http.server.begin(); server_it != http.server.end(); server_it++)
	{
		undiff(http, *server_it);
		set_default(*server_it);
		for (std::vector<Location_block>::iterator location_it = (*server_it).location.begin(); location_it != (*server_it).location.end(); location_it++)
		{
			undiff(*server_it, *location_it);
			fix_parsing_location(*location_it);
		}
	}
	std::cout << http.root << std::endl;
}
