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

#include <stddef.h>

void	print_memory(const void *addr, size_t size);

int	main(void)
{
	int		tab[10] = {0, 23, 150, 255, 12, 16, 21, 42};
	char	str[] = "Hello World! 42";
	char	tiny[] = {1, 2, 3};

	print_memory(tab, sizeof(tab));
	print_memory(str, sizeof(str));
	print_memory(tiny, sizeof(tiny));
	print_memory(str, 0);
	print_memory(str, 16);
	return (0);
}
