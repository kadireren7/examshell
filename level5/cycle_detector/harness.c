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
	int				data;
	struct s_list	*next;
}				t_list;

int	cycle_detector(const t_list *list);

static t_list	*build(int n, int loop_to)
{
	t_list	*head;
	t_list	*tail;
	t_list	*node;
	t_list	*target;
	int		i;

	head = NULL;
	tail = NULL;
	target = NULL;
	i = 0;
	while (i < n)
	{
		node = malloc(sizeof(t_list));
		node->data = i;
		node->next = NULL;
		if (!head)
			head = node;
		else
			tail->next = node;
		tail = node;
		if (i == loop_to)
			target = node;
		i++;
	}
	if (tail && loop_to >= 0)
		tail->next = target;
	return (head);
}

int	main(void)
{
	printf("%d\n", cycle_detector(NULL));
	printf("%d\n", cycle_detector(build(1, -1)));
	printf("%d\n", cycle_detector(build(5, -1)));
	printf("%d\n", cycle_detector(build(1, 0)));
	printf("%d\n", cycle_detector(build(5, 0)));
	printf("%d\n", cycle_detector(build(5, 4)));
	printf("%d\n", cycle_detector(build(5, 2)));
	printf("%d\n", cycle_detector(build(100, -1)));
	printf("%d\n", cycle_detector(build(100, 50)));
	return (0);
}
