/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 16:11:17 by jvermeer          #+#    #+#             */
/*   Updated: 2023/02/04 16:28:57 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <map>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include "server.hpp"
#include "request.hpp"

int is_CGI_php(const Request &request)
{
	size_t		ret = request.uri.find_last_of(".");

	if (ret != std::string::npos && request.uri.substr(ret, request.uri.size()) == ".php")
		return 1;
	return 0;
}

int is_CGI_python(const Request &request)
{
	size_t		ret = request.uri.find_last_of(".");

	if (ret != std::string::npos && request.uri.substr(ret, request.uri.size()) == ".py")
		return 1;
	return 0;
}

void sequential_sleep(int pid, unsigned int time, unsigned int nb, int * status)
{
	if (waitpid(pid, status, WNOHANG) == 0) // WNO = return immediately
	{
		if (nb == 0)
		{
			if (kill(pid, SIGKILL) == -1)
				perror("kill()");
		}
		usleep(time);
		sequential_sleep(pid, time, --nb, status);
	}
}

const char * return_header(const Request & request, std::map<std::string, std::string>::const_iterator it)
{
	if (it == request.headers.end())
		return "";
	return it->second.c_str();
}

void	setup_execve_attributes(std::vector<char*> & args, const Request & request, std::string & cgi, char * filename)
{
	args.push_back(const_cast<char*>(cgi.c_str()));
	args.push_back(filename);
	args.push_back(NULL);
	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("PATH_INFO", filename, 1);
	setenv("QUERY_STRING", request.query.c_str(), 1);
	setenv("REQUEST_METHOD", request.method.c_str(), 1);
	setenv("REMOTE_ADDR", request.host.c_str(), 1);
	setenv("SCRIPT_NAME", cgi.c_str(), 1);
	setenv("SCRIPT_FILENAME", filename, 1);
	setenv("SERVER_NAME", "webserv", 1);
	setenv("SERVER_PORT", request.port.c_str(), 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("SERVER_SOFTWARE", "webserv/4.2", 1);
	setenv("HTTP_USER_AGENT", return_header(request, request.headers.find("User-Agent")), 1);
	setenv("HTTP_ACCEPT", return_header(request, request.headers.find("Accept")), 1);
	setenv("HTTP_ACCEPT_LANGUAGE", return_header(request, request.headers.find("Accept-Language")), 1);
	setenv("HTTP_ACCEPT_ENCODING", return_header(request, request.headers.find("Accept-Encoding")), 1);
	setenv("REDIRECT_STATUS", "200", 1);
	if (request.method == "POST")
	{
		setenv("CONTENT_LENGTH", return_header(request, request.headers.find("Content-Length")), 1);
		setenv("CONTENT_TYPE", return_header(request, request.headers.find("Content-Type")), 1);
	}
}

int read_cgi_response(int & door0, Response & response, int & status)
{
	char buff[BUFF_SIZE];
	ssize_t ret = 1;	
	while (ret)
	{
		ret = read(door0, buff, BUFF_SIZE - 1);
		if (ret == -1)
		{
			perror("read()");
			return (set_status_code_and_return(response, "500"));
		}
		buff[ret] = '\0';
		for (ssize_t i = 0; i < ret; ++i)
			response.body += buff[i];
	}
	close(door0);
	if (status != 0)
	{
		if (response.body.find("404 Not Found") != std::string::npos)
			return (set_status_code_and_return(response, "404"));
		return (set_status_code_and_return(response, "500"));
	}
	response.status_code = "200";
	return (0);
}

int	execute_cgi(const Request & request, Response & response, std::string cgi, const char * file)
{
	std::vector<char *> args;
	char	* filename = strdup(file);
	int		status = 0;
	int		door[2];
	int		pid;
	
	if (!filename)
	{
		perror("malloc()");
		return (set_status_code_and_return(response, "500"));
	}
	if (pipe(door) == -1)
	{
		perror("pipe()");
		return (set_status_code_and_return(response, "500"));
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork()");
		return (set_status_code_and_return(response, "500"));
	}
	else if (pid == 0)
	{
		setup_execve_attributes(args, request, cgi, filename);
		extern char **environ;
		close(door[0]);
		dup2(door[1], 1);
		close(door[1]);
		execve(args[0], &args[0], environ);
		perror("execve()");
		return (set_status_code_and_return(response, "500"));
	}
	waitpid(pid, NULL, 0); /* not protected against infinite loop */
	close(door[1]);
	sequential_sleep(pid, 10000, 30, &status);/* protect our server against infinite_loop */
	free(filename);
	read_cgi_response(door[0], response, status);
	return (0);
}

void process_by_CGI(const Location_block &loc, const Request &request, Response & response)
{
	size_t		ret = request.uri.find_last_of(".");
	std::string	ext = request.uri.substr(ret, request.uri.size());

	for(std::map< std::string, std::string >::const_iterator it = loc.cgi.begin(); it != loc.cgi.end(); ++it)
	{
		if (ext == it->first)
			execute_cgi(request, response, std::string(loc.root + it->second), std::string(loc.root + request.uri).c_str());
	}
}
