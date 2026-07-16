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

static void	puthex(unsigned long n)
{
	char	*base = "0123456789abcdef";

	if (n >= 16)
		puthex(n / 16);
	put(base[n % 16]);
}

int	main(int argc, char **argv)
{
	unsigned long	n;
	int				i;

	if (argc == 2)
	{
		n = 0;
		i = 0;
		while (argv[1][i] >= '0' && argv[1][i] <= '9')
			n = n * 10 + (unsigned long)(argv[1][i++] - '0');
		puthex(n);
	}
	put('\n');
	return (0);
}
