/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamicsimplepage.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvermeer <jvermeer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/23 15:56:50 by jvermeer          #+#    #+#             */
/*   Updated: 2023/01/25 15:40:25 by jvermeer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
	char swap;
    while (start < end)
    {
		swap = str[start];
		str[start] = str[end];
		str[end] = swap;
        start++;
        end--;
    }
}
 
char * itoa(int num, char* str, int base)
{
    int i = 0;
 
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
    str[i] = '\0';
    reverse(str, i);
    return str;
}

int	generate_n(void)
{
	char buff[11];
	unsigned int num = 0;

	FILE *file;
	file = fopen("srcs/CGI/count.txt", "a+");
	if (file == NULL)
	{
		perror("fopen()");
		exit(errno);
	}
	fscanf(file, "%s", buff);
	fclose(file);
	num = atoi(buff);
	++num;
	if (num > 2147483647)
		num = 0;
	file = fopen("srcs/CGI/count.txt", "w+");
	if (file == NULL)
	{
		perror("fopen()");
		exit(errno);
	}
	fputs(itoa(num, buff, 10), file);
	fclose(file);
	return (int)num;
}

int main()
{
	printf("HTTP/1.1 200 OK\n");
	printf("Content-type: text/html\n\n");
	printf("<html>\n");
	printf("<body>\n");
	printf("<h1>Hello there!</h1>\n");
	printf("<h1>You are the %dth visitor</h1>\n", generate_n());
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}