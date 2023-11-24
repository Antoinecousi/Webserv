/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_loop.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 16:11:17 by jvermeer          #+#    #+#             */
/*   Updated: 2023/02/04 21:34:29 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <list>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <arpa/inet.h>
#include <cstdlib>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdio.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "Http_block.hpp"
#include "response.hpp"
#include "Server_block.hpp"
#include "server.hpp"

int	server_loop(const Http_block &http)
{
	std::vector<t_server> servers;
	std::list<t_client>	clients;
	char buffer[BUFF_SIZE];
	struct timeval to;
	fd_set readfds;
	fd_set writefds;
	size_t y = 0;
	int ret;

	std::set< std::pair<std::string, std::string> >	multibind;
	for (std::vector<Server_block>::const_iterator server_it = http.server.begin(); server_it != http.server.end(); server_it++)
	{
		for (std::vector< std::pair<std::string, std::string> >::const_iterator it = server_it->listen.begin(); it != server_it->listen.end(); it++)
		{
			if (it->first == "0.0.0.0")
			{
				if (multibind.find(std::make_pair(it->first, it->second)) == multibind.end())
				{
					multibind.insert(std::make_pair(it->first, it->second));
					addServSock(servers, it->first.c_str(), std::atoi(it->second.c_str()));
				}
			}
		}
	}
	for (std::vector<Server_block>::const_iterator server_it = http.server.begin(); server_it != http.server.end(); server_it++)
	{
		for (std::vector< std::pair<std::string, std::string> >::const_iterator it = server_it->listen.begin(); it != server_it->listen.end(); it++)
		{
			if (multibind.find(std::make_pair("0.0.0.0", it->second)) == multibind.end() && multibind.find(std::make_pair(it->first, it->second)) == multibind.end())
			{
				multibind.insert(std::make_pair(it->first, it->second));
				addServSock(servers, it->first.c_str(), std::atoi(it->second.c_str()));
			}
		}
	}

	int max = 0;
	if (servers.size()) /* if there isn't any IP/port pair */
		max = (servers.end() - 1)->socket;
	
	/* Infinite loop: iterates on each server */
	while (y < servers.size())
	{
		try {
			/* empty the readfds set */
			FD_ZERO(&readfds);
			FD_ZERO(&writefds);
			
			/* timeval attributes are modified after each select() call... */
			to.tv_sec = 0;
			to.tv_usec = 200;

			#ifdef DEBUG
			FD_SET(STDIN_FILENO, &readfds);
			#endif

			/* add server socket, for new connections */
			FD_SET(servers[y].socket, &readfds);

			/* add clients sockets et read and write fd_sets, so they will be check by select() */
			for (std::list<t_client>::iterator it = clients.begin(); it != clients.end(); ++it)
			{
				FD_SET(it->socket, &readfds);
				FD_SET(it->socket, &writefds);
			}

			/* select() use 'max' variable as a hint of when it can stop its linear scan of the file descriptor table */
			/* Could set 'max' to the max fd value supported by the system, but = inefficiencies */
			if (select(max + 1, &readfds, &writefds, (fd_set *)0, &to) == SOCKET_ERROR)
				perror("select()");

			#ifdef DEBUG
			if (FD_ISSET(STDIN_FILENO, &readfds))
			{
				read(STDIN_FILENO, buffer, BUFF_SIZE - 1);
				break;
			}
			#endif
			
			/* check whether there is new connection on servers[y] */
			if (FD_ISSET(servers[y].socket, &readfds))
			{
				t_client newclient;
				
				
				/* Accept connection, start TCP flow */
				newclient.socket = accept(servers[y].socket, NULL, NULL);
				/* 2nd and 3rd arguments can be used to restrict connection on specific clients	*/
				/* ** SOCKADDR_IN clientsin = { 0 };											*/
				/* ** unsigned int csinsize = sizeof(clientsin);								*/
				if (newclient.socket == SOCKET_ERROR)
					perror("accept()");

				//std::cout << "New Connection " << newclient.socket << "\n\n";

				/* Set the socket as 'non-blocking' */
				fcntl(newclient.socket, F_SETFL, O_NONBLOCK);

				/* Update the biggest fd -> hint for select() */
				max = newclient.socket > max ? newclient.socket : max;

				/* Add new client to our std::list */
				newclient.sin = servers[y].sin;
				clients.push_back(newclient);
			}
			
			/* check whether the server can read to write to one client */
			for (std::list<t_client>::iterator it = clients.begin(); it != clients.end(); ++it)
			{
				if (FD_ISSET(it->socket, &readfds))
				{
					ret = read(it->socket, buffer, BUFF_SIZE - 1);
					if (ret == -1)
					{
						perror("read()");
						closesocket(it->socket);
						it = clients.erase(it);
					}
					else if (ret == 0) /* 0 -> client close connection */
					{
						//std::cout << "Close: " << it->socket << "\n\n";
						closesocket(it->socket);
						it = clients.erase(it);
					}
					else if (it->request.capacity() < 209715200)
					{
						try
						{
							it->request.reserve(it->request.size() + ret);
						}
						catch (...)
						{
							it->request = '\0';
							// internal error incoming
						}
						for (int i = 0; i < ret; ++i)
							it->request += buffer[i];
					}
					break;
				}
				else if (!it->request.empty() && FD_ISSET(it->socket, &writefds))
				{
					/* GENERATE RESPONSE */
					if (it->response.empty()) /* else, a response has already been generated but not fully sent */
					{
						it->keep_alive = 1;
						try
						{
							process_request(http, std::string(inet_ntop(AF_INET, &it->sin.sin_addr, buffer, BUFF_SIZE)), std::string(INT_TO_STRING(ntohs(it->sin.sin_port))), it->request, it->response, it->keep_alive);
						}
						catch (std::exception & e)
						{
							std::cerr << e.what() << std::endl;
						}
					}
					signal(SIGPIPE, SIG_IGN);
					ret = write(it->socket, &it->response[0], it->response.size());
					if (ret == -1)
					{
						perror("write()");
						closesocket(it->socket);
						it = clients.erase(it);
					}
					else if (ret >= 0 && (size_t)ret < it->response.size())
						it->response.erase(it->response.begin(), it->response.begin() + ret);
					else
					{
						it->request.clear(); /* signals to server_loop that the request has been processed */
						
						/* if "Connection: keep-alive" */
						if (it->keep_alive)
							shutdown(it->socket, SHUT_WR);
						else
						{
							closesocket(it->socket);
							it = clients.erase(it);				
						}		
					}
					break;
				}
			}
		}
		catch (std::exception & e)
		{
			std::cerr << e.what() << std::endl;
		}
		if (++y >= servers.size())
			y = 0;
	}
	/* Close all fds in case someone wrote in STDIN */
	#ifdef DEBUG
	for (y = 0; y < servers.size(); ++y)
	{
		for (std::list<t_client>::iterator it = clients.begin(); it != clients.end(); ++it)
		{
			closesocket(it->socket);
		}
		//std::cout << "close serv: " << y << std::endl;
		closesocket(servers[y].socket);
	}
	#endif
	return (0);
}
