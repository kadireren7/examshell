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

#include <unistd.h>
static void	put(char c)
{
	write(1, &c, 1);
}

static int	is_sp(char c)
{
	return (c == ' ' || c == '\t');
}

static char	up(char c)
{
	if (c >= 'a' && c <= 'z')
		return ((char)(c - 32));
	return (c);
}

static char	low(char c)
{
	if (c >= 'A' && c <= 'Z')
		return ((char)(c + 32));
	return (c);
}

int	main(int argc, char **argv)
{
	int	a;
	int	i;
	int	last;

	if (argc < 2)
	{
		put('\n');
		return (0);
	}
	a = 1;
	while (a < argc)
	{
		i = 0;
		while (argv[a][i])
		{
			last = (!argv[a][i + 1] || is_sp(argv[a][i + 1]));
			if (is_sp(argv[a][i]))
				put(argv[a][i]);
			else
				put(last ? up(argv[a][i]) : low(argv[a][i]));
			i++;
		}
		put('\n');
		a++;
	}
	return (0);
}
