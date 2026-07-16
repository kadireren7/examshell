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

#include <unistd.h>

void	print_bits(unsigned char octet);

int	main(void)
{
	unsigned int	i;
	unsigned char	t[] = {0, 1, 2, 3, 42, 64, 127, 128, 170, 255};

	i = 0;
	while (i < sizeof(t) / sizeof(t[0]))
	{
		print_bits(t[i]);
		write(1, "\n", 1);
		i++;
	}
	return (0);
}
