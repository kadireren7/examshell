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

int	ft_atoi_base(const char *str, int str_base);

int	main(void)
{
	printf("%d\n", ft_atoi_base("1010", 2));
	printf("%d\n", ft_atoi_base("123", 4));
	printf("%d\n", ft_atoi_base("ff", 16));
	printf("%d\n", ft_atoi_base("FF", 16));
	printf("%d\n", ft_atoi_base("12fdb3", 16));
	printf("%d\n", ft_atoi_base("12FDB3", 16));
	printf("%d\n", ft_atoi_base("-42", 10));
	printf("%d\n", ft_atoi_base("42", 10));
	printf("%d\n", ft_atoi_base("-101", 2));
	printf("%d\n", ft_atoi_base("0", 10));
	printf("%d\n", ft_atoi_base("", 10));
	printf("%d\n", ft_atoi_base("777", 8));
	printf("%d\n", ft_atoi_base("12z34", 10));
	printf("%d\n", ft_atoi_base("129", 8));
	printf("%d\n", ft_atoi_base("z", 16));
	return (0);
}
