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

static int	is_prime(int n)
{
	int	i;

	if (n < 2)
		return (0);
	i = 2;
	while ((long)i * i <= n)
	{
		if (n % i == 0)
			return (0);
		i++;
	}
	return (1);
}

static int	parse(char *s, int *ok)
{
	int	n;

	n = 0;
	*ok = 1;
	if (!*s)
		*ok = 0;
	while (*s)
	{
		if (*s < '0' || *s > '9')
		{
			*ok = 0;
			return (0);
		}
		n = n * 10 + (*s - '0');
		s++;
	}
	return (n);
}

int	main(int argc, char **argv)
{
	int	n;
	int	ok;
	int	i;
	long	sum;

	sum = 0;
	if (argc == 2)
	{
		n = parse(argv[1], &ok);
		if (ok)
		{
			i = 2;
			while (i <= n)
			{
				if (is_prime(i))
					sum += i;
				i++;
			}
		}
	}
	putnbr(sum);
	put('\n');
	return (0);
}
