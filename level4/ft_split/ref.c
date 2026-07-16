/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ref.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 06:01:37 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 06:01:37 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static int	is_sp(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	count_words(char *s)
{
	int	n;
	int	i;

	n = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && is_sp(s[i]))
			i++;
		if (s[i])
			n++;
		while (s[i] && !is_sp(s[i]))
			i++;
	}
	return (n);
}

char	**ft_split(char *str)
{
	char	**res;
	int		i;
	int		w;
	int		start;
	int		j;

	res = malloc(sizeof(char *) * (unsigned long)(count_words(str) + 1));
	if (!res)
		return (0);
	i = 0;
	w = 0;
	while (str[i])
	{
		while (str[i] && is_sp(str[i]))
			i++;
		if (!str[i])
			break ;
		start = i;
		while (str[i] && !is_sp(str[i]))
			i++;
		res[w] = malloc((unsigned long)(i - start) + 1);
		if (!res[w])
			return (0);
		j = 0;
		while (start + j < i)
		{
			res[w][j] = str[start + j];
			j++;
		}
		res[w][j] = '\0';
		w++;
	}
	res[w] = 0;
	return (res);
}
