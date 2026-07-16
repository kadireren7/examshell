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

int	main(int argc, char **argv)
{
	char	seen[256];
	int		i;

	i = 0;
	while (i < 256)
		seen[i++] = 0;
	if (argc == 3)
	{
		i = 0;
		while (argv[1][i])
		{
			unsigned char	c = (unsigned char)argv[1][i];
			int				j = 0;

			while (argv[2][j] && argv[2][j] != argv[1][i])
				j++;
			if (argv[2][j] && !seen[c])
			{
				seen[c] = 1;
				put((char)c);
			}
			i++;
		}
	}
	put('\n');
	return (0);
}
