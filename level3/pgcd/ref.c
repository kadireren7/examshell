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

#include <stdio.h>
#include <stdlib.h>

int	main(int argc, char **argv)
{
	int	a;
	int	b;
	int	t;

	if (argc != 3)
	{
		printf("\n");
		return (0);
	}
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	while (b)
	{
		t = b;
		b = a % b;
		a = t;
	}
	printf("%d\n", a);
	return (0);
}
