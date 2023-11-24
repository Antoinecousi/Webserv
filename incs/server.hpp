/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 07:36:53 by jvermeer          #+#    #+#             */
/*   Updated: 2023/01/30 20:52:48 by jvermeer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <list>
#include <netinet/ip.h>
#include <string>
#include <sys/socket.h>
#include <vector>
#include <Http_block.hpp>
#include "Location_block.hpp"
#include "request.hpp"
#include "response.hpp"

# define SERVLOOP
# define SOCKET_ERROR -1
# define BUFF_SIZE 1024

typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

typedef struct	s_client
{
	int			socket;
	bool		keep_alive;
	std::string response;
	std::string request;
	SOCKADDR_IN sin;
}				t_client;

typedef struct	s_server
{
	int			    socket;
	SOCKADDR_IN     sin;
}				t_server;

int 	is_CGI_php(const Request &request);
int 	is_CGI_python(const Request &request);
void 	process_by_CGI(const Location_block &loc, const Request &request, Response & response);
int		server_loop(const Http_block &http);
void	closesocket(int fd);
void	addServSock(std::vector<t_server> &servers, const char * IPaddr, unsigned short port);

#endif
