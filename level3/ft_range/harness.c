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

int	*ft_range(int start, int end);

static void	one(int a, int b)
{
	int	*r;
	int	n;
	int	i;

	r = ft_range(a, b);
	n = (b >= a ? b - a : a - b) + 1;
	i = 0;
	while (i < n)
	{
		printf("%d ", r[i]);
		i++;
	}
	printf("\n");
	free(r);
}

int	main(void)
{
	one(1, 3);
	one(-1, 2);
	one(0, 0);
	one(0, -3);
	one(5, 5);
	one(-3, -1);
	one(10, 1);
	return (0);
}
