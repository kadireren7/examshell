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

char	**ft_split(char *str);

static void	one(char *s)
{
	char	**r;
	int		i;

	r = ft_split(s);
	if (!r)
	{
		printf("NULL\n");
		return ;
	}
	i = 0;
	while (r[i])
	{
		printf("[%s]", r[i]);
		i++;
	}
	printf(" count:%d\n", i);
}

int	main(void)
{
	char	a[] = "hello world";
	char	b[] = "   leading and trailing   ";
	char	c[] = "";
	char	d[] = "     ";
	char	e[] = "one";
	char	f[] = "tabs\tand\nnewlines\tmixed";
	char	g[] = "a b  c   d";

	one(a);
	one(b);
	one(c);
	one(d);
	one(e);
	one(f);
	one(g);
	return (0);
}
