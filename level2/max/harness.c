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

int	max(int *tab, unsigned int len);

int	main(void)
{
	int	a[] = {1, 2, 3, 4, 5};
	int	b[] = {5, 4, 3, 2, 1};
	int	c[] = {-5, -4, -3};
	int	d[] = {42};
	int	e[] = {2147483647, -2147483648, 0};
	int	f[] = {7, 7, 7};

	printf("%d\n", max(a, 5));
	printf("%d\n", max(b, 5));
	printf("%d\n", max(c, 3));
	printf("%d\n", max(d, 1));
	printf("%d\n", max(e, 3));
	printf("%d\n", max(f, 3));
	printf("%d\n", max(a, 0));
	return (0);
}
