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

int	ft_strcmp(char *s1, char *s2);

static int	norm(int n)
{
	if (n > 0)
		return (1);
	if (n < 0)
		return (-1);
	return (0);
}

static void	one(char *a, char *b)
{
	printf("%d\n", norm(ft_strcmp(a, b)));
}

int	main(void)
{
	char	x[] = "abc";
	char	y[] = "abc";

	one(x, y);
	one("abc", "abd");
	one("abd", "abc");
	one("", "");
	one("", "a");
	one("a", "");
	one("abc", "ab");
	one("ab", "abc");
	one("ABC", "abc");
	one("test\200", "test\1");
	return (0);
}
