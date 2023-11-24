/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   choose_location.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: acousini <acousini@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 10:35:34 by acousini          #+#    #+#             */
/*   Updated: 2023/02/04 20:42:24 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Http_block.hpp"
#include "Server_block.hpp"
#include "Location_block.hpp"
#include "request.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <dirent.h>

static int getLevelServerBlock(const Server_block &server, const std::string &ip_request, const std::string &host)
{
    int i;
    int max;
    std::vector<int>level;

	i = 0;
	max = 0;
	
    while (static_cast<size_t>(i) < server.listen.size())
    {
		level.push_back(0);
        // check if the IP address matches the server's IP address
        if (server.listen[i].first == ip_request.substr(0, ip_request.find(':')) ||
            server.listen[i].first == "0.0.0.0")
            level[i] += 4;
        else
            level[i] -= 10;
        // check if the port number matches the server's port number
        if (server.listen[i].second == ip_request.substr(ip_request.find(':')+1))
            level[i] += 4;
        else
            level[i] -= 10;
        // check if the hostname matches the server's hostname
		if (server.listen[i].first == "0.0.0.0")
		{
			level[i] -= 2;
		}
		if (server.get_flag_server_name())
		{
			if (server.server_name[i] == host)
				level[i] += 1;
		}
        if (max < level[i])
            max = level[i];
        i++;
    }
    i = 0;
    while (static_cast<unsigned long>(i) < level.size())
    {
        if (level[i] == max)
            return (level[i]);
        i++;
    }
    return (max);
}

int getLevelLocationBlock(std::string uri, const Location_block loc)
{
    int i;
    int j;
    int level;
	int	charMatched;
	int totalCharMatched;
    std::string locUri;

	charMatched = 0;
	totalCharMatched = 0;
	i = 0;
	j = 0;
	level = 0;
	locUri = loc.uri;
    // compare the URI with the location URI
    while (static_cast<unsigned long>(i) < uri.size() &&
			static_cast<unsigned long>(j) < locUri.size())
    {
        if (uri[i] == locUri[j])
        {
			// check if end of directory name by matching '/' or end of both reqURI and locURI or next char of reqURI is '/'
			if (i == 0 && static_cast<unsigned long>(j) + 1 == locUri.size())
			{
				level++;
				totalCharMatched += charMatched;
				charMatched = 0;
			}
            else if (uri[i] == '/' && (i != 0 ||
				(locUri.size() - 1 == static_cast<unsigned long>(j) && (uri.size() - 1 == static_cast<unsigned long>(i) || uri[i + 1] == '/'))))
			{
				level += 2;
				totalCharMatched += charMatched;
				charMatched = 0;
			}
			else if ((locUri.size() - 1 == static_cast<unsigned long>(j) && uri[i + 1] == '/') ||
						(uri.size() - 1 == static_cast<unsigned long>(i) && locUri[j + 1] == '/') ||
						(uri.size() - 1 == static_cast<unsigned long>(i) && locUri.size() - 1 == static_cast<unsigned long>(j)))
			{
				level += 2;
				totalCharMatched += charMatched;
				charMatched = 0;
			}
        }
		else
			break ;
        i++;
        j++;
    }
    // check if the location requires an absolute path
    if ((locUri.size() != static_cast<unsigned long>(j) || uri.size() != static_cast<unsigned long>(i)) && loc.equal_modifier == true) // check w/ '/' & without '/' at then finish
	{
        level = 0;
	}
	if (static_cast<unsigned long>(j) < locUri.size())
	{
		level = 0;
	}
    return (level);
}

void pickExactLocation(const Location_block &loc, std::string uri, const Location_block **result)
{
    int levelLocation = 0;
    int bestMatchLength = 0;
    const Location_block *bestMatch = 0;

    for (size_t count = 0; count < loc.location.size(); ++count)
	{
        const Location_block &location = loc.location[count];
        levelLocation = getLevelLocationBlock(uri, location);
        if (levelLocation > bestMatchLength)
		{
            bestMatch = &location;
            bestMatchLength = levelLocation;
        }
        if (location.equal_modifier == true && levelLocation > 0)
		{
            *result = &location;
            return ;
        }
    }
    if (bestMatch != 0  && bestMatch->get_flag_location() == true)
        pickExactLocation(*bestMatch, uri, result);
    else
	{
		if (bestMatch != 0)
			*result = bestMatch;
		else
			*result = &loc;
	}
}

const Location_block *pickExactLocationWrapper(const Location_block &loc, std::string uri)
{
    const Location_block *result = 0;
    pickExactLocation(loc, uri, &result);
    return result;
}

const Location_block &pickLocation(const Http_block &http, const Request &request)
{
    int bestMatchLength;
    int servPicked;
    int levelLocation;
	int max;
    std::vector<int>level;
    const Location_block *bestMatch;
	std::string port;
	std::string server_name;

	bestMatchLength = 0;
	levelLocation = 0;
	servPicked = 0;
	bestMatch = 0;
	max = 0;
	server_name = request.headers.at("Host").substr(0, request.headers.at("Host").find(':'));
	port = request.host + ":" + request.port;
    for (size_t count = 0; count < http.server.size(); ++count) 
	{
		level.push_back(0);
    	const Server_block &server = http.server[count];
        level[count] = getLevelServerBlock(server, port, server_name);
        if (level[count] > max)
		{
			max = level[count];
            servPicked = count;
		}
    }
    //find the best matching location
	for (size_t count = 0; count < http.server[servPicked].location.size(); ++count)
	{
		const Location_block &location = http.server[servPicked].location[count];
		levelLocation = getLevelLocationBlock(request.uri, location);
		if (location.equal_modifier == true && levelLocation > 0)
			return (location);
		if (levelLocation > bestMatchLength)
		{
			bestMatch = &location;
			bestMatchLength = levelLocation;
		}
    }
	if (bestMatch && bestMatch->get_flag_location() == true)
		bestMatch = pickExactLocationWrapper(*bestMatch, request.uri);
	return (*bestMatch);
}
