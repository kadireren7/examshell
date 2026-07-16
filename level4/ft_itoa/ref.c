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

static int	len_of(int n)
{
	int	len;

	len = (n <= 0);
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

char	*ft_itoa(int nbr)
{
	char			*s;
	int				len;
	unsigned int	n;

	len = len_of(nbr);
	s = malloc((unsigned long)len + 1);
	if (!s)
		return (0);
	s[len] = '\0';
	if (nbr < 0)
	{
		s[0] = '-';
		n = (unsigned int)(-(long)nbr);
	}
	else
		n = (unsigned int)nbr;
	if (n == 0)
		s[0] = '0';
	while (n)
	{
		s[--len] = (char)('0' + n % 10);
		n /= 10;
	}
	return (s);
}
