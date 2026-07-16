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

static int	is_piece(char c)
{
	return (c == 'K' || c == 'P' || c == 'B' || c == 'R' || c == 'Q');
}

static int	ray(char **b, int size, int r, int c, int dr, int dc)
{
	r += dr;
	c += dc;
	while (r >= 0 && r < size && c >= 0 && c < size)
	{
		if (is_piece(b[r][c]))
			return (b[r][c]);
		r += dr;
		c += dc;
	}
	return (0);
}

/*
** Walk out from the king along every capture pattern. Only the first piece
** met on a ray matters; anything further is blocked by it.
*/
static int	in_check(char **b, int size, int kr, int kc)
{
	int	d;
	int	diag[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
	int	line[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

	d = 0;
	while (d < 4)
	{
		int	hit = ray(b, size, kr, kc, diag[d][0], diag[d][1]);

		if (hit == 'B' || hit == 'Q')
			return (1);
		hit = ray(b, size, kr, kc, line[d][0], line[d][1]);
		if (hit == 'R' || hit == 'Q')
			return (1);
		d++;
	}
	if (kr + 1 < size && kc - 1 >= 0 && b[kr + 1][kc - 1] == 'P')
		return (1);
	if (kr + 1 < size && kc + 1 < size && b[kr + 1][kc + 1] == 'P')
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	int	size;
	int	r;
	int	c;
	int	kr;
	int	kc;

	if (argc < 2)
	{
		put('\n');
		return (0);
	}
	size = argc - 1;
	kr = -1;
	kc = -1;
	r = 0;
	while (r < size)
	{
		c = 0;
		while (argv[r + 1][c])
		{
			if (argv[r + 1][c] == 'K')
			{
				kr = r;
				kc = c;
			}
			c++;
		}
		r++;
	}
	if (kr < 0)
	{
		puts_("Fail");
		put('\n');
		return (0);
	}
	puts_(in_check(argv + 1, size, kr, kc) ? "Success" : "Fail");
	put('\n');
	return (0);
}
