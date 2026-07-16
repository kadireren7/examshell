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

int	ft_strlen(char *str);

int	main(void)
{
	char	buf[] = "mutable";

	printf("%d\n", ft_strlen(""));
	printf("%d\n", ft_strlen("a"));
	printf("%d\n", ft_strlen("Hello World!"));
	printf("%d\n", ft_strlen(buf));
	printf("%d\n", ft_strlen("  spaces  and\ttabs\n"));
	printf("%d\n", ft_strlen("0123456789012345678901234567890123456789"));
	return (0);
}
