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

#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv)
{
	int	n;
	int	d;
	int	first;

	if (argc != 2)
	{
		printf("\n");
		return (0);
	}
	n = atoi(argv[1]);
	if (n <= 1)
	{
		printf("%d\n", n);
		return (0);
	}
	first = 1;
	d = 2;
	while ((long)d * d <= n)
	{
		while (n % d == 0)
		{
			printf(first ? "%d" : "*%d", d);
			first = 0;
			n /= d;
		}
		d++;
	}
	if (n > 1)
		printf(first ? "%d" : "*%d", n);
	printf("\n");
	return (0);
}
