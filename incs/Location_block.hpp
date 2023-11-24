/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location_block.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/10 13:11:01 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/26 18:14:35 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_BLOCK_HPP
# define LOCATION_BLOCK_HPP

# include <string>
# include <vector>
# include <map>
# include <utility>

class Server_block;
class Location_block
{
	private :

	bool	_flag_error_page;
	bool	_flag_client_max_body_size;
	bool	_flag_location;
	bool	_flag_limit_except;
	bool	_flag_return_;
	bool	_flag_root;
	bool	_flag_autoindex;
	bool	_flag_index;
	bool	_flag_cgi;

	public :

	std::string							uri;
	bool								equal_modifier;
	std::map<int, std::string>			error_page;
	long long							client_max_body_size;
	std::vector<Location_block>			location;
	std::vector<std::string>			limit_except;
	std::pair<int, std::string>			return_;
	std::string							root;
	bool								autoindex;
	std::vector<std::string>			index;
	std::map<std::string, std::string>	cgi;

	Location_block(const std::string &uri_value, const bool equal_modifier_value);

	void	set_flag_error_page(bool value);
	void	set_flag_client_max_body_size(bool value);
	void	set_flag_location(bool value);
	void	set_flag_limit_except(bool value);
	void	set_flag_return_(bool value);
	void	set_flag_root(bool value);
	void	set_flag_autoindex(bool value);
	void	set_flag_index(bool value);
	void	set_flag_cgi(bool value);

	const bool	&get_flag_error_page(void) const;
	const bool	&get_flag_client_max_body_size(void) const;
	const bool	&get_flag_location(void) const;
	const bool	&get_flag_limit_except(void) const;
	const bool	&get_flag_return_(void) const;
	const bool	&get_flag_root(void) const;
	const bool	&get_flag_autoindex(void) const;
	const bool	&get_flag_index(void) const;
	const bool	&get_flag_cgi(void) const;
};

#endif
