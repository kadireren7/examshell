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
#include <stdlib.h>

char	*ft_itoa(int nbr);

int	main(void)
{
	int		t[] = {0, 1, -1, 9, 10, -10, 42, -42, 100, 2147483647,
		-2147483647, -2147483648, 123456789};
	size_t	i;
	char	*s;

	i = 0;
	while (i < sizeof(t) / sizeof(t[0]))
	{
		s = ft_itoa(t[i]);
		printf("[%s]\n", s ? s : "NULL");
		free(s);
		i++;
	}
	return (0);
}
