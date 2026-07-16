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

/*
** Expand around every centre and keep the longest run. Ties go to the later
** palindrome, so the comparison is >= and not >.
*/
static void	best(char *s, int *bs, int *bl)
{
	int	i;
	int	len;

	*bs = 0;
	*bl = 0;
	len = 0;
	while (s[len])
		len++;
	i = 0;
	while (i < len)
	{
		int	half;

		half = 0;
		while (i - half >= 0 && i + half < len
			&& s[i - half] == s[i + half])
			half++;
		half--;
		if (2 * half + 1 >= *bl)
		{
			*bl = 2 * half + 1;
			*bs = i - half;
		}
		half = 0;
		while (i - half >= 0 && i + 1 + half < len
			&& s[i - half] == s[i + 1 + half])
			half++;
		half--;
		if (half >= 0 && 2 * half + 2 >= *bl)
		{
			*bl = 2 * half + 2;
			*bs = i - half;
		}
		i++;
	}
}

int	main(int argc, char **argv)
{
	int	start;
	int	len;
	int	i;

	if (argc != 2)
	{
		put('\n');
		return (0);
	}
	best(argv[1], &start, &len);
	i = 0;
	while (i < len)
		put(argv[1][start + i++]);
	put('\n');
	return (0);
}
