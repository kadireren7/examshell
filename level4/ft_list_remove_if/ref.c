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
#include "ft_list.h"

void	ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)())
{
	t_list	*cur;

	if (!begin_list)
		return ;
	while (*begin_list && cmp((*begin_list)->data, data_ref) == 0)
	{
		cur = *begin_list;
		*begin_list = cur->next;
		free(cur);
	}
	if (!*begin_list)
		return ;
	cur = *begin_list;
	while (cur->next)
	{
		if (cmp(cur->next->data, data_ref) == 0)
		{
			t_list	*dead = cur->next;

			cur->next = dead->next;
			free(dead);
		}
		else
			cur = cur->next;
	}
}
