/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_block.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/02/03 16:51:31 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_BLOCK_HPP
# define SERVER_BLOCK_HPP

# include <string>
# include <vector>
# include <map>
# include <utility>
# include "Location_block.hpp"

class Http_block;
class Server_block
{
	private :

	bool	_flag_listen;
	bool	_flag_server_name;
	bool	_flag_error_page;
	bool	_flag_client_max_body_size;
	bool	_flag_location;
	bool	_flag_return_;
	bool	_flag_root;
	bool	_flag_autoindex;
	bool	_flag_index;

	public :

	std::vector< std::pair<std::string, std::string> >	listen;
	std::vector<std::string>							server_name;
	std::map<int, std::string>							error_page;
	long long											client_max_body_size;
	std::vector<Location_block>							location;
	std::pair<int, std::string>							return_;
	std::string											root;
	bool												autoindex;
	std::vector<std::string>							index;

	Server_block(void);

	void	set_flag_listen(bool value);
	void	set_flag_server_name(bool value);
	void	set_flag_error_page(bool value);
	void	set_flag_client_max_body_size(bool value);
	void	set_flag_location(bool value);
	void	set_flag_return_(bool value);
	void	set_flag_root(bool value);
	void	set_flag_autoindex(bool value);
	void	set_flag_index(bool value);

	const bool	&get_flag_listen(void) const;
	const bool	&get_flag_server_name(void) const;
	const bool	&get_flag_error_page(void) const;
	const bool	&get_flag_client_max_body_size(void) const;
	const bool	&get_flag_location(void) const;
	const bool	&get_flag_return_(void) const;
	const bool	&get_flag_root(void) const;
	const bool	&get_flag_autoindex(void) const;
	const bool	&get_flag_index(void) const;
};

#endif
