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
#include <stddef.h>

static void	put(char c)
{
	write(1, &c, 1);
}

static void	puthex2(unsigned char b)
{
	char	*d = "0123456789abcdef";

	put(d[b >> 4]);
	put(d[b & 15]);
}

/*
** Sixteen bytes a line: eight two-byte groups of hex, then the same bytes as
** text. A short last line still pads its hex columns so the text stays put.
*/
void	print_memory(const void *addr, size_t size)
{
	const unsigned char	*p = addr;
	size_t				i;
	size_t				j;

	i = 0;
	while (i < size)
	{
		j = 0;
		while (j < 16)
		{
			if (i + j < size)
				puthex2(p[i + j]);
			else
			{
				put(' ');
				put(' ');
			}
			if (j % 2 == 1)
				put(' ');
			j++;
		}
		j = 0;
		while (j < 16 && i + j < size)
		{
			if (p[i + j] >= 32 && p[i + j] <= 126)
				put((char)p[i + j]);
			else
				put('.');
			j++;
		}
		put('\n');
		i += 16;
	}
}
