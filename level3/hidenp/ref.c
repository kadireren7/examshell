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

#include <unistd.h>
static void	put(char c)
{
	write(1, &c, 1);
}

int	main(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 3)
	{
		put('\n');
		return (0);
	}
	i = 0;
	j = 0;
	while (argv[1][i] && argv[2][j])
	{
		if (argv[1][i] == argv[2][j])
			i++;
		j++;
	}
	put(argv[1][i] ? '0' : '1');
	put('\n');
	return (0);
}
