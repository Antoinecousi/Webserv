/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_parsing.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 15:41:53 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/23 13:11:50 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <vector>
#include <stack>
#include <utility>
#include <string>
#include "Http_block.hpp"
#include "parsing.hpp"

int	core_parsing(Http_block &http, std::ifstream &ifs)
{
	std::stack< std::pair<e_context, void *> >	contexts;
	std::vector<std::string>					tokens;
	std::vector<std::string>::const_iterator	token;
	std::string::size_type						l;
	bool										http_flag;

	// Default context
	contexts.push(std::pair<e_context, void *>(MAIN, NULL));
	l = 0;
	http_flag = false;
	while (extract_tokens(ifs, tokens, token, l) == 0)
	{
		while (token != tokens.end())
		{
			if (*token == "http")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_context(ifs, directive, args, contexts.top().first, MAIN, tokens, token, l) < 0)
					return (-1);
				if (args.size() != 0)
					return (invalid_number_of_arguments_error(directive, l));
				if (http_flag)
					return (directive_is_duplicate_error(directive, l));
				http_flag = true;
				parse_http(contexts, http);
			}
			else if (*token == "server")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_context(ifs, directive, args, contexts.top().first, HTTP, tokens, token, l) < 0)
					return (-1);
				if (args.size() != 0)
					return (invalid_number_of_arguments_error(directive, l));
				parse_server(contexts);
			}
			else if (*token == "location")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_context(ifs, directive, args, contexts.top().first, SERVER | LOCATION, tokens, token, l) < 0)
					return (-1);
				if (args.size() < 1 || 2 < args.size())
					return (invalid_number_of_arguments_error(directive, l));
				if (parse_location(contexts, directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "}")
			{
				contexts.pop();
			}
			else if (*token == "listen")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(ifs, directive, args, contexts.top().first, SERVER, tokens, token, l) < 0)
					return (-1);
				if (args.size() != 1)
					return (invalid_number_of_arguments_error(directive, l));
				if (parse_listen(contexts.top(), directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "server_name")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(ifs, directive, args, contexts.top().first, SERVER, tokens, token, l) < 0)
					return (-1);
				if (args.size() < 1)
					return (invalid_number_of_arguments_error(directive, l));
				if (parse_server_name(contexts.top(), args) < 0)
					return (-1);
			}
			else if (*token == "error_page")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(ifs, directive, args, contexts.top().first, HTTP | SERVER | LOCATION, tokens, token, l) < 0)
					return (-1);
				if (args.size() < 2)
					return (invalid_number_of_arguments_error(directive, l));
				if (parse_error_page(contexts.top(), directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "client_max_body_size")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(ifs, directive, args, contexts.top().first, HTTP | SERVER | LOCATION, tokens, token, l) < 0)
					return (-1);
				if (args.size() != 1)
					return (invalid_number_of_arguments_error(directive, l));
				if (is_duplicate_client_max_body_size(contexts.top()))
					return (directive_is_duplicate_error(directive, l));
				if (parse_client_max_body_size(contexts.top(), directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "limit_except")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(ifs, directive, args, contexts.top().first, LOCATION, tokens, token, l) < 0)
					return (-1);
				if (args.size() < 1)
					return (invalid_number_of_arguments_error(directive, l));
				if (parse_limit_except(contexts.top(), directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "return")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(ifs, directive, args, contexts.top().first, SERVER | LOCATION, tokens, token, l) < 0)
					return (-1);
				if (args.size() < 1 || 2 < args.size())
					return (invalid_number_of_arguments_error(directive, l));
				if (is_duplicate_return(contexts.top()))
					return (directive_is_duplicate_error(directive, l));
				if (parse_return(contexts.top(), directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "root")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(ifs, directive, args, contexts.top().first, HTTP | SERVER | LOCATION, tokens, token, l) < 0)
					return (-1);
				if (args.size() != 1)
					return (invalid_number_of_arguments_error(directive, l));
				if (is_duplicate_root(contexts.top()))
					return (directive_is_duplicate_error(directive, l));
				if (parse_root(contexts.top(), directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "autoindex")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(ifs, directive, args, contexts.top().first, HTTP | SERVER | LOCATION, tokens, token, l) < 0)
					return (-1);
				if (args.size() != 1)
					return (invalid_number_of_arguments_error(directive, l));
				if (is_duplicate_autoindex(contexts.top()))
					return (directive_is_duplicate_error(directive, l));
				if (parse_autoindex(contexts.top(), directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "index")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(ifs, directive, args, contexts.top().first, HTTP | SERVER | LOCATION, tokens, token, l) < 0)
					return (-1);
				if (args.size() < 1)
					return (invalid_number_of_arguments_error(directive, l));
				if (parse_index(contexts.top(), directive, args, l) < 0)
					return (-1);
			}
			else if (*token == "cgi")
			{
				std::string					directive;
				std::vector<std::string>	args;

				if (parse_directive(ifs, directive, args, contexts.top().first, LOCATION, tokens, token, l) < 0)
					return (-1);
				if (args.size() < 2)
					return (invalid_number_of_arguments_error(directive, l));
				if (parse_cgi(contexts.top(), directive, args, l) < 0)
					return (-1);
			}
			else
				return (unknown_directive_error(*token, l));
			token++;
		}
	}
	fix_parsing(http);
	if (!(ifs.eof()))
		return (-1);
	return (0);
}
