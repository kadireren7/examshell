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

static char	*put_nbr(char *p, unsigned int n)
{
	if (n >= 10)
		p = put_nbr(p, n / 10);
	*p++ = (char)('0' + n % 10);
	return (p);
}

static char	*put_str(char *p, const char *s)
{
	while (*s)
		*p++ = *s++;
	return (p);
}

char	*moment(unsigned int duration)
{
	const char		*unit;
	unsigned int	n;
	char			*res;
	char			*p;

	if (duration < 60)
	{
		n = duration;
		unit = "second";
	}
	else if (duration < 3600)
	{
		n = duration / 60;
		unit = "minute";
	}
	else if (duration < 86400)
	{
		n = duration / 3600;
		unit = "hour";
	}
	else if (duration < 2592000)
	{
		n = duration / 86400;
		unit = "day";
	}
	else
	{
		n = duration / 2592000;
		unit = "month";
	}
	res = malloc(64);
	if (!res)
		return (0);
	p = put_nbr(res, n);
	*p++ = ' ';
	p = put_str(p, unit);
	if (n != 1)
		*p++ = 's';
	p = put_str(p, " ago.");
	*p = '\0';
	return (res);
}
