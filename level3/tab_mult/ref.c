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

static void	putnbr(long n)
{
	if (n < 0)
	{
		put('-');
		n = -n;
	}
	if (n >= 10)
		putnbr(n / 10);
	put((char)('0' + n % 10));
}

int	main(int argc, char **argv)
{
	int	n;
	int	i;

	if (argc != 2)
	{
		put('\n');
		return (0);
	}
	n = 0;
	i = 0;
	while (argv[1][i] >= '0' && argv[1][i] <= '9')
		n = n * 10 + (argv[1][i++] - '0');
	i = 1;
	while (i <= 9)
	{
		putnbr(i);
		put(' ');
		put('x');
		put(' ');
		putnbr(n);
		put(' ');
		put('=');
		put(' ');
		putnbr((long)i * n);
		put('\n');
		i++;
	}
	return (0);
}
