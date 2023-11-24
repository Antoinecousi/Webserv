/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simpleform.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/24 17:26:36 by jvermeer          #+#    #+#             */
/*   Updated: 2023/01/24 17:32:50 by jvermeer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("Content-type: text/html\n\n");
	printf("<html>\n");
	printf("<body>\n");
	printf("<h1>The value entered was: %s</h1>\n", getenv("PATH_INFO"));
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}