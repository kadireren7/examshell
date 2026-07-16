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

char	*ft_strrev(char *str);

static void	one(char *s)
{
	char	*r;

	r = ft_strrev(s);
	printf("[%s] ret==arg:%d\n", s, r == s);
}

int	main(void)
{
	char	a[] = "";
	char	b[] = "a";
	char	c[] = "ab";
	char	d[] = "Hello World!";
	char	e[] = "racecar";

	one(a);
	one(b);
	one(c);
	one(d);
	one(e);
	return (0);
}
