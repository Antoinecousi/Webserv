/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http_block.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:10:58 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_BLOCK_HPP
# define HTTP_BLOCK_HPP

# include <string>
# include <vector>
# include <map>
# include "Server_block.hpp"

class Http_block
{
	private :

	bool	_flag_error_page;
	bool	_flag_client_max_body_size;
	bool	_flag_server;
	bool	_flag_root;
	bool	_flag_autoindex;
	bool	_flag_index;

	public :

	std::map<int, std::string>			error_page;
	long long							client_max_body_size;
	std::vector<Server_block>					server;
	std::string							root;
	bool								autoindex;
	std::vector<std::string>			index;

	Http_block(void);

	void	set_flag_error_page(bool value);
	void	set_flag_client_max_body_size(bool value);
	void	set_flag_server(bool value);
	void	set_flag_root(bool value);
	void	set_flag_autoindex(bool value);
	void	set_flag_index(bool value);

	const bool	&get_flag_error_page(void) const;
	const bool	&get_flag_client_max_body_size(void) const;
	const bool	&get_flag_server(void) const;
	const bool	&get_flag_root(void) const;
	const bool	&get_flag_autoindex(void) const;
	const bool	&get_flag_index(void) const;
};

#endif
