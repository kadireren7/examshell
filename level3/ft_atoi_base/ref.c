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

static int	digit(char c, int base)
{
	int	v;

	v = -1;
	if (c >= '0' && c <= '9')
		v = c - '0';
	else if (c >= 'a' && c <= 'f')
		v = c - 'a' + 10;
	else if (c >= 'A' && c <= 'F')
		v = c - 'A' + 10;
	if (v >= base)
		return (-1);
	return (v);
}

int	ft_atoi_base(const char *str, int str_base)
{
	int	sign;
	int	res;
	int	d;

	sign = 1;
	res = 0;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	d = digit(*str, str_base);
	while (*str && d >= 0)
	{
		res = res * str_base + d;
		str++;
		d = digit(*str, str_base);
	}
	return (res * sign);
}
