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

int	is_power_of_2(unsigned int n);

int	main(void)
{
	unsigned int	t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 16, 31, 32, 33, 64,
		1024, 1023, 2147483648u, 4294967295u};
	size_t			i;

	i = 0;
	while (i < sizeof(t) / sizeof(t[0]))
	{
		printf("%u -> %d\n", t[i], is_power_of_2(t[i]));
		i++;
	}
	return (0);
}
