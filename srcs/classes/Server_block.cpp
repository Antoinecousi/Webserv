/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server_block.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 03:13:21 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/19 14:10:22 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "Http_block.hpp"
#include "Server_block.hpp"
#include "Location_block.hpp"

// Constructor
Server_block::Server_block(void)
{
	_flag_listen = false;
	_flag_server_name = false;
	_flag_error_page = false;
	_flag_client_max_body_size = false;
	_flag_location = false;
	_flag_return_ = false;
	_flag_root = false;
	_flag_autoindex = false;
	_flag_index = false;
}

// Setters
void	Server_block::set_flag_listen(bool value)
{
	this->_flag_listen = value;
}

void	Server_block::set_flag_server_name(bool value)
{
	this->_flag_server_name = value;
}

void	Server_block::set_flag_error_page(bool value)
{
	this->_flag_error_page = value;
}

void	Server_block::set_flag_client_max_body_size(bool value)
{
	this->_flag_client_max_body_size = value;
}

void	Server_block::set_flag_location(bool value)
{
	this->_flag_location = value;
}

void	Server_block::set_flag_return_(bool value)
{
	this->_flag_return_ = value;
}

void	Server_block::set_flag_root(bool value)
{
	this->_flag_root = value;
}

void	Server_block::set_flag_autoindex(bool value)
{
	this->_flag_autoindex = value;
}

void	Server_block::set_flag_index(bool value)
{
	this->_flag_index = value;
}

// Getters
const bool	&Server_block::get_flag_listen(void) const
{
	return (this->_flag_listen);
}

const bool	&Server_block::get_flag_server_name(void) const
{
	return (this->_flag_server_name);
}

const bool	&Server_block::get_flag_error_page(void) const
{
	return (this->_flag_error_page);
}

const bool	&Server_block::get_flag_client_max_body_size(void) const
{
	return (this->_flag_client_max_body_size);
}

const bool	&Server_block::get_flag_location(void) const
{
	return (this->_flag_location);
}

const bool	&Server_block::get_flag_return_(void) const
{
	return (this->_flag_return_);
}

const bool	&Server_block::get_flag_root(void) const
{
	return (this->_flag_root);
}

const bool	&Server_block::get_flag_autoindex(void) const
{
	return (this->_flag_autoindex);
}

const bool	&Server_block::get_flag_index(void) const
{
	return (this->_flag_index);
}
