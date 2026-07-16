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

static int	is_sp(char c)
{
	return (c == ' ' || c == '\t');
}

int	main(int argc, char **argv)
{
	int	i;
	int	fs;
	int	fe;
	int	first;

	if (argc < 2)
	{
		put('\n');
		return (0);
	}
	i = 0;
	while (is_sp(argv[1][i]))
		i++;
	fs = i;
	while (argv[1][i] && !is_sp(argv[1][i]))
		i++;
	fe = i;
	first = 1;
	while (argv[1][i])
	{
		while (is_sp(argv[1][i]))
			i++;
		if (!argv[1][i])
			break ;
		if (!first)
			put(' ');
		first = 0;
		while (argv[1][i] && !is_sp(argv[1][i]))
			put(argv[1][i++]);
	}
	if (fs < fe)
	{
		if (!first)
			put(' ');
		while (fs < fe)
			put(argv[1][fs++]);
	}
	put('\n');
	return (0);
}
