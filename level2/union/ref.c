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

static void	pass(char *s, char *seen)
{
	int	i;

	i = 0;
	while (s[i])
	{
		unsigned char	c = (unsigned char)s[i];

		if (!seen[c])
		{
			seen[c] = 1;
			put((char)c);
		}
		i++;
	}
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
		pass(argv[1], seen);
		pass(argv[2], seen);
	}
	put('\n');
	return (0);
}
