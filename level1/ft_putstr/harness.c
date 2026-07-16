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

void	ft_putstr(char *str);

int	main(void)
{
	char	mutable[] = "a string that can be written into";

	ft_putstr("Hello World!");
	write(1, "|", 1);
	ft_putstr("");
	write(1, "|", 1);
	ft_putstr("42");
	write(1, "|", 1);
	ft_putstr(mutable);
	write(1, "|", 1);
	ft_putstr("with\ttab and\nnewline");
	write(1, "|\n", 2);
	return (0);
}
