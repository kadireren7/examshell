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

unsigned char	reverse_bits(unsigned char octet)
{
	unsigned char	res;
	int				i;

	res = 0;
	i = 0;
	while (i < 8)
	{
		res = (res << 1) | ((octet >> i) & 1);
		i++;
	}
	return (res);
}
