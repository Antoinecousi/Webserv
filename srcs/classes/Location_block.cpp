/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location_block.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <spayeur@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 03:13:21 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/23 13:09:39 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <vector>
#include <map>
#include <unistd.h>
#include "Server_block.hpp"
#include "Location_block.hpp"

// Constructor
Location_block::Location_block(const std::string &uri_value, const bool equal_modifier_value)
{
	_flag_error_page = false;
	_flag_client_max_body_size = false;
	_flag_location = false;
	_flag_limit_except = false;
	_flag_return_ = false;
	_flag_root = false;
	_flag_autoindex = false;
	_flag_index = false;
	_flag_cgi = false;

	uri = uri_value;
	equal_modifier = equal_modifier_value;
}

// Setters
void	Location_block::set_flag_error_page(bool value)
{
	this->_flag_error_page = value;
}

void	Location_block::set_flag_client_max_body_size(bool value)
{
	this->_flag_client_max_body_size = value;
}

void	Location_block::set_flag_location(bool value)
{
	this->_flag_location = value;
}

void	Location_block::set_flag_limit_except(bool value)
{
	this->_flag_limit_except = value;
}

void	Location_block::set_flag_return_(bool value)
{
	this->_flag_return_ = value;
}

void	Location_block::set_flag_root(bool value)
{
	this->_flag_root = value;
}

void	Location_block::set_flag_autoindex(bool value)
{
	this->_flag_autoindex = value;
}

void	Location_block::set_flag_index(bool value)
{
	this->_flag_index = value;
}

void	Location_block::set_flag_cgi(bool value)
{
	this->_flag_cgi = value;
}

// Getters
const bool	&Location_block::get_flag_error_page(void) const
{
	return (this->_flag_error_page);
}

const bool	&Location_block::get_flag_client_max_body_size(void) const
{
	return (this->_flag_client_max_body_size);
}

const bool	&Location_block::get_flag_location(void) const
{
	return (this->_flag_location);
}

const bool	&Location_block::get_flag_limit_except(void) const
{
	return (this->_flag_limit_except);
}

const bool	&Location_block::get_flag_return_(void) const
{
	return (this->_flag_return_);
}

const bool	&Location_block::get_flag_root(void) const
{
	return (this->_flag_root);
}

const bool	&Location_block::get_flag_autoindex(void) const
{
	return (this->_flag_autoindex);
}

const bool	&Location_block::get_flag_index(void) const
{
	return (this->_flag_index);
}

const bool	&Location_block::get_flag_cgi(void) const
{
	return (this->_flag_cgi);
}
