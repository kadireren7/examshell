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

int	main(int argc, char **argv)
{
	int	i;
	int	end;
	int	first;

	if (argc != 2)
	{
		put('\n');
		return (0);
	}
	i = 0;
	while (argv[1][i])
		i++;
	i--;
	first = 1;
	while (i >= 0)
	{
		while (i >= 0 && is_sp(argv[1][i]))
			i--;
		if (i < 0)
			break ;
		end = i;
		while (i >= 0 && !is_sp(argv[1][i]))
			i--;
		if (!first)
			put(' ');
		first = 0;
		{
			int	j = i + 1;

			while (j <= end)
				put(argv[1][j++]);
		}
	}
	put('\n');
	return (0);
}
