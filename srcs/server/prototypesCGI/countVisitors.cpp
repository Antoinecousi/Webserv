/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   countVisitors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/25 16:48:09 by jvermeer          #+#    #+#             */
/*   Updated: 2023/01/25 17:20:58 by jvermeer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

int main()
{
	size_t	numb;

	std::fstream file;
	file.open("srcs/CGI/count.txt", std::ios::out | std::ios::in);
	file >> numb;
	file.close();
	++numb;
	if (numb > 2147483647)
		numb = 0;
	file.open("srcs/CGI/count.txt", std::ios::out | std::ios::in | std::ios::trunc);
	file << numb;
	file.close();
	std::cout << "HTTP/1.1 200 OK\nContent-type: text/html\n\n<html>\n<body>\n<h1>You are the " << numb << "th visitor</h1>\n</body>\n</html>\n" << std::endl;
	return 0;
}