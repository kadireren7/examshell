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

static void	puts_(const char *s)
{
	while (*s)
		put(*s++);
}

static void	usage(void)
{
	puts_("options: abcdefghijklmnopqrstuvwxyz\n");
}

static void	show(unsigned int bits)
{
	int	byte;
	int	bit;

	byte = 3;
	while (byte >= 0)
	{
		bit = 7;
		while (bit >= 0)
		{
			put((char)('0' + ((bits >> (byte * 8 + bit)) & 1)));
			bit--;
		}
		if (byte)
			put(' ');
		byte--;
	}
	put('\n');
}

int	main(int argc, char **argv)
{
	unsigned int	bits;
	int				i;
	int				j;

	if (argc < 2)
	{
		usage();
		return (0);
	}
	bits = 0;
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] == '-')
		{
			j = 1;
			while (argv[i][j])
			{
				if (argv[i][j] < 'a' || argv[i][j] > 'z')
				{
					puts_("Invalid Option\n");
					return (0);
				}
				bits |= 1u << (argv[i][j] - 'a');
				j++;
			}
		}
		i++;
	}
	if (bits & (1u << ('h' - 'a')))
		usage();
	else
		show(bits);
	return (0);
}
