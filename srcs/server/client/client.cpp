/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/11 17:43:20 by jvermeer          #+#    #+#             */
/*   Updated: 2023/01/19 14:43:54 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>

# define SOCKET_ERROR -1
# define PORT 8080
# define h_addr  h_addr_list[0] /* reference man hostent: pour retro-compabilité */
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

void closesocket(int fd)
{
	if (close(fd) == SOCKET_ERROR)
		perror("close()");
}

IN_ADDR get_address(const char *hostname)
{
	struct hostent *hostinfo = NULL;
    
	hostinfo = gethostbyname(hostname); /* on récupère les informations de l'hôte auquel on veut se connecter */
	if (hostinfo == NULL)
	{
	    fprintf (stderr, "Unknown host %s.\n", hostname);
	    exit(h_errno);
	}
    return *(IN_ADDR *)hostinfo->h_addr; /* CAST h_addr, defined as h_addr_list[0]* TO in_addr*  */
}

void setbuff(char *buffer, const char *str)
{
	while (*str)
		*buffer++ = *str++;
	*buffer = '\0';
}

int returnNewSock()
{
	/* create one "client" endpoint for the connection */
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == SOCKET_ERROR)
	{
	    perror("socket()");
	    exit(errno);
	}

	/* set up sin with target informations */
	SOCKADDR_IN server;
	bzero(&server, sizeof(server));
	server.sin_addr = get_address("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	
	if (connect(sock, (SOCKADDR *) &server, sizeof(server)) == SOCKET_ERROR) /* CAST sockaddr_in* TO sockadd*  */
	{
	    perror("connect()");
	    exit(errno);
	}
	return sock;
}

int main()
{
	std::string ret;
	int sock;

	// fcntl(sock, F_SETFL, O_NONBLOCK);

	char buf1[2048] = "";
	char buf2[2048] = "MY REQUEST = 1023 char\nGET / HTTP/1.1\nHost: 127.0.0.1:8080\nConnection: 2\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nsec-ch-ua: \"Google Chrome\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"\nsec-ch-ua-mobile: ?0\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\n";
	char buf3[2048] = "MY REQUEST = 1024 char\nGET / HTTP/1.1\nHost: 127.0.0.1:8080\nConnection: 20\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nConnection: keep-alive\nsec-ch-ua: \"Google Chrome\";v=\"107\", \"Chromium\";v=\"107\", \"Not=A?Brand\";v=\"24\"\nsec-ch-ua-mobile: ?0\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\nsec-ch-ua-platform: \"Linux\"\n";
	while (1)
	{
		std::cin >> ret;
		if (ret == "CLOSE")
			closesocket(sock);
		if (ret == "NEW")
			sock = returnNewSock();
		else if (ret == "1")
		{
			std::cout << "send empty\n";
			if (send(sock, buf1, strlen(buf1), 0) < 0)
			    perror("send()");
		}
		else if (ret == "2")
		{
			std::cout << "send read size\n";
			if (send(sock, buf2, strlen(buf2), 0) < 0)
			    perror("send()");
		}
		else if (ret == "3")
		{
			std::cout << "send (read size + 1)\n";
			if (send(sock, buf3, strlen(buf3), 0) < 0)
			    perror("send()");
		}
		usleep(1000);
	}
	return(0);
}
