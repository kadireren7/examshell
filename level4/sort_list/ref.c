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

#include "list.h"

t_list	*sort_list(t_list *lst, int (*cmp)(int, int))
{
	t_list	*cur;
	int		tmp;
	int		swapped;

	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		cur = lst;
		while (cur && cur->next)
		{
			if (!(*cmp)(cur->data, cur->next->data))
			{
				tmp = cur->data;
				cur->data = cur->next->data;
				cur->next->data = tmp;
				swapped = 1;
			}
			cur = cur->next;
		}
	}
	return (lst);
}
