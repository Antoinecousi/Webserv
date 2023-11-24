/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: spayeur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 20:16:43 by spayeur           #+#    #+#             */
/*   Updated: 2023/01/27 20:23:12 by spayeur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <map>
#include "response.hpp"

// Statics constructors
const std::map<std::string, std::string>	Response::status_code_definitions = Response::status_code_definitions_constructor();
const std::map<std::string, std::string>	Response::mime_types = Response::mime_types_constructor();
