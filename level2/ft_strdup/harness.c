/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   harness.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 06:01:37 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 06:01:37 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char	*ft_strdup(char *src);

static void	one(char *s)
{
	char	*d;

	d = ft_strdup(s);
	if (!d)
	{
		printf("NULL\n");
		return ;
	}
	printf("[%s] len:%zu distinct:%d\n", d, strlen(d), d != s);
	free(d);
}

int	main(void)
{
	char	mut[] = "mutable string";

	one("");
	one("a");
	one("Hello World!");
	one(mut);
	one("with\ttab and spaces   ");
	return (0);
}
