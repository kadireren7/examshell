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
	(void)argv;
	putnbr(argc - 1);
	put('\n');
	return (0);
}
