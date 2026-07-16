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

void	sort_int_tab(int *tab, unsigned int size);

static void	one(int *tab, unsigned int n)
{
	unsigned int	i;

	sort_int_tab(tab, n);
	i = 0;
	while (i < n)
		printf("%d ", tab[i++]);
	printf("\n");
}

int	main(void)
{
	int	a[] = {5, 4, 3, 2, 1};
	int	b[] = {1, 2, 3};
	int	c[] = {3, 1, 3, 1, 2};
	int	d[] = {42};
	int	e[] = {0, -1, 2147483647, -2147483648, 7};
	int	f[] = {1};

	one(a, 5);
	one(b, 3);
	one(c, 5);
	one(d, 1);
	one(e, 5);
	one(f, 0);
	return (0);
}
