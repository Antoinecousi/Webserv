/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 07:28:16 by jvermeer          #+#    #+#             */
/*   Updated: 2023/01/28 21:56:56 by jvermeer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <list>
#include <vector>
#include <string>

#include "server.hpp"

void closesocket(int fd)
{
	if (close(fd) == SOCKET_ERROR)
		perror("close()");
}

void addServSock(std::vector<t_server> &servers, const char * IPaddr, unsigned short port)
{
	t_server	serv;
	int			addr;
	
	/* inet_pton() converts a human-readable IP address into its binary representation in network byte order (= big-endian) */
	int	ret = inet_pton(AF_INET, IPaddr, &addr);
	if (ret < 0)
	{
		perror("inet_pton()");
		exit(errno);
	}
	else if (ret == 0)
	{
		std::cerr << "inet_pton(): " << IPaddr << " isn't a correct IPv4 format\n";
		exit(1);
	}
	
	/* set up one "server" endpoint for the connection */
	serv.socket = socket(AF_INET, SOCK_STREAM, 0);
	if (serv.socket == SOCKET_ERROR)
    {
		perror("socket()");
		exit(errno);
    }

	/* When a server binds to a port, the operating system kernel reserves that port for the exclusive use of the server */
	/* The SO_REUSEADDR option allows another socket to bind to the same port as soon as the server's socket is closed */
	/* (It will work if the port is busy in the TIME_WAIT state. If it is busy but with another state, you will still get an address already in use error) */
	/* setsock = 1 (to enable reuse of the address) or setsock = 0 (to disable reuse of the address) */
	int	setsock = 1;
	if (setsockopt(serv.socket, SOL_SOCKET, SO_REUSEADDR, &setsock, sizeof(int)) < 0)
    {
		perror("setsockopt()");
		exit(errno);
    }

	/* create sin struct to set up our "server" socket's attributes */
	bzero(&serv.sin, sizeof(serv.sin));
	serv.sin.sin_family = AF_INET;	 /* IPv4 */
	serv.sin.sin_port = htons(port); /* htons convert unsigned short integer, from IPaddr byte order TO network byte order: always big-endian */
	serv.sin.sin_addr.s_addr = addr;

	/* We bind the socket to an IP and PORT where it can then listen() for connections */
	if (bind(serv.socket, (SOCKADDR *)&serv.sin, sizeof(serv.sin)) == SOCKET_ERROR) /* CAST sockaddr_in* TO sockadd*  */
	{
		perror("bind()");
		exit(errno);
	}
	if (listen(serv.socket, 15) == SOCKET_ERROR) /* second arg = backlog / queue */
	{
		perror("listen()");
		exit(errno);
	}
	servers.push_back(serv);
}
