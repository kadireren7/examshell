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

char	*ft_itoa_base(int value, int base);

static void	one(int v, int b)
{
	char	*s;

	s = ft_itoa_base(v, b);
	printf("%d base %d = [%s]\n", v, b, s ? s : "NULL");
	free(s);
}

int	main(void)
{
	one(0, 10);
	one(42, 10);
	one(-42, 10);
	one(42, 2);
	one(42, 16);
	one(255, 16);
	one(-1, 16);
	one(-1, 2);
	one(2147483647, 10);
	one(-2147483648, 10);
	one(-2147483648, 16);
	one(7, 8);
	one(255, 4);
	one(1, 2);
	one(-5, 10);
	return (0);
}
