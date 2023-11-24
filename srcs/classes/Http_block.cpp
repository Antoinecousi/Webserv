/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Http_block.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 03:13:21 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/22 12:18:26 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "Http_block.hpp"
#include "Server_block.hpp"

// Constructor
Http_block::Http_block(void)
{
	_flag_error_page = false;
	_flag_client_max_body_size = false;
	_flag_server = false;
	_flag_root = false;
	_flag_autoindex = false;
	_flag_index = false;
}

// Setters
void	Http_block::set_flag_error_page(bool value)
{
	this->_flag_error_page = value;
}

void	Http_block::set_flag_client_max_body_size(bool value)
{
	this->_flag_client_max_body_size = value;
}

void	Http_block::set_flag_server(bool value)
{
	this->_flag_server = value;
}

void	Http_block::set_flag_root(bool value)
{
	this->_flag_root = value;
}

void	Http_block::set_flag_autoindex(bool value)
{
	this->_flag_autoindex = value;
}

void	Http_block::set_flag_index(bool value)
{
	this->_flag_index = value;
}

// Getters
const bool	&Http_block::get_flag_error_page(void) const
{
	return (this->_flag_error_page);
}

const bool	&Http_block::get_flag_client_max_body_size(void) const
{
	return (this->_flag_client_max_body_size);
}

const bool	&Http_block::get_flag_server(void) const
{
	return (this->_flag_server);
}

const bool	&Http_block::get_flag_root(void) const
{
	return (this->_flag_root);
}

const bool	&Http_block::get_flag_autoindex(void) const
{
	return (this->_flag_autoindex);
}

const bool	&Http_block::get_flag_index(void) const
{
	return (this->_flag_index);
}
