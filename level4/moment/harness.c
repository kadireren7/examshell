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

char	*moment(unsigned int duration);

int	main(void)
{
	unsigned int	t[] = {0, 1, 2, 30, 59, 60, 65, 120, 2400, 3599, 3600,
		3735, 7200, 86399, 86400, 172800, 2591999, 2592000, 5184000,
		31536000};
	size_t			i;
	char			*s;

	i = 0;
	while (i < sizeof(t) / sizeof(t[0]))
	{
		s = moment(t[i]);
		printf("%u => %s\n", t[i], s ? s : "NULL");
		free(s);
		i++;
	}
	return (0);
}
