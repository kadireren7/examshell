/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   harness.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 06:01:37 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 06:01:37 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

typedef struct	s_list
{
	struct s_list	*next;
	void			*data;
}				t_list;

int	ft_list_size(t_list *begin_list);

static t_list	*build(int n)
{
	t_list	*head;
	t_list	*node;
	int		i;

	head = NULL;
	i = 0;
	while (i < n)
	{
		node = malloc(sizeof(t_list));
		node->data = NULL;
		node->next = head;
		head = node;
		i++;
	}
	return (head);
}

int	main(void)
{
	int	sizes[] = {0, 1, 2, 5, 42};
	size_t	i;

	i = 0;
	while (i < sizeof(sizes) / sizeof(sizes[0]))
	{
		printf("%d\n", ft_list_size(build(sizes[i])));
		i++;
	}
	return (0);
}
