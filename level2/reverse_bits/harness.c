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

unsigned char	reverse_bits(unsigned char octet);

int	main(void)
{
	unsigned int	i;
	unsigned char	t[] = {0, 1, 2, 65, 42, 128, 170, 255, 16, 7};

	i = 0;
	while (i < sizeof(t) / sizeof(t[0]))
	{
		printf("%u -> %u\n", t[i], reverse_bits(t[i]));
		i++;
	}
	return (0);
}
