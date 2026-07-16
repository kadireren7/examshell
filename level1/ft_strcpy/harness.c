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

char	*ft_strcpy(char *s1, char *s2);

static void	one(char *src)
{
	char	dst[128];
	char	*ret;
	int		i;

	i = 0;
	while (i < 128)
		dst[i++] = '#';
	ret = ft_strcpy(dst, src);
	printf("[%s] ret==dst:%d\n", dst, ret == dst);
}

int	main(void)
{
	char	mut[] = "mutable source";

	one("");
	one("a");
	one("Hello World!");
	one(mut);
	one("with\ttab");
	return (0);
}
