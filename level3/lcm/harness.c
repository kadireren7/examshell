/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   harness.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 06:01:37 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 06:01:37 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>

unsigned int	lcm(unsigned int a, unsigned int b);

int	main(void)
{
	unsigned int	t[][2] = {{2, 3}, {4, 6}, {5, 5}, {1, 1}, {0, 5}, {5, 0},
		{0, 0}, {12, 18}, {21, 6}, {100, 75}, {17, 5}};
	size_t			i;

	i = 0;
	while (i < sizeof(t) / sizeof(t[0]))
	{
		printf("%u\n", lcm(t[i][0], t[i][1]));
		i++;
	}
	return (0);
}
