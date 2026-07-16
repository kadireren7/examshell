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

char	*ft_itoa_base(int value, int base)
{
	char			*digits = "0123456789ABCDEF";
	char			buf[64];
	char			*res;
	int				i;
	int				len;
	unsigned int	n;
	int				neg;

	neg = (base == 10 && value < 0);
	if (neg)
		n = (unsigned int)(-(long)value);
	else
		n = (unsigned int)value;
	i = 0;
	if (n == 0)
		buf[i++] = '0';
	while (n)
	{
		buf[i++] = digits[n % (unsigned int)base];
		n /= (unsigned int)base;
	}
	len = i + neg;
	res = malloc((unsigned long)len + 1);
	if (!res)
		return (0);
	res[len] = '\0';
	if (neg)
		res[0] = '-';
	while (i > 0)
	{
		res[len - i] = buf[i - 1];
		i--;
	}
	return (res);
}
