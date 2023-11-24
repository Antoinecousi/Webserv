/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/09 16:17:03 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/23 14:25:52 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>

int	unexpected_character_error(const char character, const std::string::size_type l)
{
	std::cerr << "unexpected '" << character << "' (line " << l << ")" << std::endl;
	return (-1);
}

int	unexpected_end_of_file_error(const std::string::size_type l)
{
	std::cerr << "unexpected end of file, expecting ';' or '}' (line " << l << ")" << std::endl;
	return (-1);
}

int	unknown_directive_error(const std::string &directive, const std::string::size_type l)
{
	std::cerr << "unknown directive '" << directive << "' (line " << l << ")" << std::endl;
	return (-1);
}

int	directive_not_allowed_here_error(const std::string &directive, const std::string::size_type l)
{
	std::cerr << "'" << directive << "' directive is not allowed here (line " << l << ")" << std::endl;
	return (-1);
}

int	directive_has_no_opening_error(const std::string &directive, const std::string::size_type l)
{
	std::cerr << "directive '" << directive << "' has no opening '{' (line " << l << ")" << std::endl;
	return (-1);
}

int	invalid_number_of_arguments_error(const std::string &directive, const std::string::size_type l)
{
	std::cerr << "invalid number of arguments in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	directive_is_duplicate_error(const std::string &directive, const std::string::size_type l)
{
	std::cerr << "'" << directive << "' directive is duplicate (line " << l << ")" << std::endl;
	return (-1);
}

int	too_long_argument_error(const std::string &directive, const std::string::size_type l, const std::string &short_path)
{
	std::cerr << "too long argument '" << short_path << "' started in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	invalid_value_error(const std::string &directive, const std::string::size_type l, const std::string &value)
{
	std::cerr << "invalid value '" << value << "' in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	value_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &value, const long long min, const long long max, const std::string &unit)
{
	std::cerr << "value '" << value << unit << "' must be between " << min << unit << " and " << max << unit << " in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	invalid_location_modifier_error(const std::string &directive, const std::string::size_type l, const std::string &modifier)
{
	std::cerr << "invalid location modifier '" << modifier << "' in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	location_cannot_be_inside_the_exact_location_error(const std::string &directive, const std::string::size_type l, const std::string &sublocation, const std::string &location)
{
	std::cerr << "location '" << sublocation << "' cannot be inside the exact location '" << location << "' in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	duplicate_location_error(const std::string &directive, const std::string::size_type l, const std::string &location, const std::string &aka)
{
	if (location == aka)
		std::cerr << "duplicate location '" << location << "' in '" << directive << "' directive (line " << l << ")" << std::endl;
	else
		std::cerr << "duplicate location '" << location << "' (aka '" << aka << "') in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	location_is_outside_location_error(const std::string &directive, const std::string::size_type l, const std::string &sublocation, const std::string &aka, const std::string &location)
{
	if (sublocation == aka)
		std::cerr << "location '" << sublocation << "' is outside location '" << location << "' in '" << directive << "' directive (line " << l << ")" << std::endl;
	else
		std::cerr << "location '" << sublocation << "' (aka '" << aka << "') is outside location '" << location << "' in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	invalid_host_error(const std::string &directive, const std::string::size_type l, const std::string &value)
{
	std::cerr << "invalid host in '" << value << "' of the '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	host_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &host, const long long min, const long long max)
{
	std::cerr << "host '" << host << "' bytes must be between " << min << " and " << max << " in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

std::string	host_not_found_error(const std::string &directive, const std::string::size_type l, const std::string &host_port)
{
	std::cerr << "host not found in '" << host_port << "' of the '" << directive << "' directive (line " << l << ")" << std::endl;
	return (std::string());
}

int	invalid_port_error(const std::string &directive, const std::string::size_type l, const std::string &value)
{
	std::cerr << "invalid port in '" << value << "' of the '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	port_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &port, const long long min, const long long max)
{
	std::cerr << "port '" << port << "' must be between " << min << " and " << max << " in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	duplicate_listening_error(const std::string &directive, const std::string::size_type l, const std::string &host, const std::string &port)
{
	std::cerr << "duplicate listening on '" << host << ":" << port << "' in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	invalid_method_error(const std::string &directive, const std::string::size_type l, const std::string &method)
{
	std::cerr << "invalid method '" << method << "' in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	invalid_return_code_error(const std::string &directive, const std::string::size_type l, const std::string &code)
{
	std::cerr << "invalid return code '" << code << "' in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

int	return_code_must_be_between_error(const std::string &directive, const std::string::size_type l, const std::string &code, const long long min, const long long max)
{
	std::cerr << "return code '" << code << "' must be between " << min << " and " << max << " in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}

std::string	symbolic_link_loop_error(const std::string &directive, const std::string::size_type l, const std::string &path)
{
	std::cerr << "symbolic link loop detected in path '" << path << "' of the '" << directive << "' directive (line " << l << ")" << std::endl;
	return (std::string());
}

int too_long_path_after_resolution_error(const std::string &directive, const std::string::size_type l, const std::string &short_path)
{
	std::cerr << "too long path '" << short_path << "' after resolution started in '" << directive << "' directive (line " << l << ")" << std::endl;
	return (-1);
}
