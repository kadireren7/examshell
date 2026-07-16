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

void	ft_swap(int *a, int *b);

static void	one(int x, int y)
{
	ft_swap(&x, &y);
	printf("%d %d\n", x, y);
}

int	main(void)
{
	int	same;

	one(1, 2);
	one(-42, 42);
	one(0, 0);
	one(2147483647, -2147483648);
	same = 7;
	ft_swap(&same, &same);
	printf("%d\n", same);
	return (0);
}
