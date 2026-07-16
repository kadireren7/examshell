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

static void	put(const char *s)
{
	int	i = 0;

	while (s[i])
		i++;
	write(1, s, i);
}

static void	putnbr(int n)
{
	char	c;

	if (n >= 10)
		putnbr(n / 10);
	c = '0' + n % 10;
	write(1, &c, 1);
}

int	main(void)
{
	int	i = 1;

	while (i <= 100)
	{
		if (i % 15 == 0)
			put("fizzbuzz");
		else if (i % 3 == 0)
			put("fizz");
		else if (i % 5 == 0)
			put("buzz");
		else
			putnbr(i);
		put("\n");
		i++;
	}
	return (0);
}
