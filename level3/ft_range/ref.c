/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ref.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 06:01:37 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 06:01:37 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	*ft_range(int start, int end)
{
	int	*tab;
	int	len;
	int	i;
	int	step;

	step = (start <= end) ? 1 : -1;
	len = (start <= end ? end - start : start - end) + 1;
	tab = malloc(sizeof(int) * (unsigned long)len);
	if (!tab)
		return (0);
	i = 0;
	while (i < len)
	{
		tab[i] = start + i * step;
		i++;
	}
	return (tab);
}
