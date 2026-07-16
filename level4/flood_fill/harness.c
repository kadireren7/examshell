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
#include <string.h>

typedef struct	s_point {
	int			x;
	int			y;
}				t_point;

void	flood_fill(char **tab, t_point size, t_point begin);

static char	**make_area(char **zone, int h)
{
	char	**area;
	int		i;

	area = malloc(sizeof(char *) * (size_t)h);
	i = 0;
	while (i < h)
	{
		area[i] = strdup(zone[i]);
		i++;
	}
	return (area);
}

static void	show(char **area, int h)
{
	int	i;

	i = 0;
	while (i < h)
		printf("%s\n", area[i++]);
	printf("--\n");
}

static void	one(char **zone, int w, int h, int bx, int by)
{
	char	**area;
	t_point	size;
	t_point	begin;

	size.x = w;
	size.y = h;
	begin.x = bx;
	begin.y = by;
	area = make_area(zone, h);
	flood_fill(area, size, begin);
	show(area, h);
}

int	main(void)
{
	char	*z1[] = {"11111111", "10001001", "10010001", "10110001",
		"11100001"};
	char	*z2[] = {"11111111", "10001001", "10010001", "10110001",
		"11100001"};
	char	*z3[] = {"....", "....", "....", "...."};
	char	*z4[] = {"1"};

	one(z1, 8, 5, 2, 2);
	one(z2, 8, 5, 0, 0);
	one(z3, 4, 4, 1, 1);
	one(z4, 1, 1, 0, 0);
	return (0);
}
