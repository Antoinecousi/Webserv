/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simplewebpage.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 15:56:50 by jvermeer          #+#    #+#             */
/*   Updated: 2023/01/23 18:35:24 by jvermeer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"

int main()
{
	// printf("HTTP/1.1 200 OK\nContent-type: text/html\n\n<html>\n<body>\n<h1>Hello there!</h1>\n</body>\n</html>\n");
	printf("HTTP/1.1 200 OK\n");
	printf("Content-type: text/html\n\n");
	printf("<html>\n");
	printf("<body>\n");
	printf("<h1>Hello there!</h1>\n");
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}