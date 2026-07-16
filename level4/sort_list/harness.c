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

t_list	*sort_list(t_list *lst, int (*cmp)(int, int));

static int	ascending(int a, int b)
{
	return (a <= b);
}

static int	descending(int a, int b)
{
	return (a >= b);
}

static t_list	*build(int *v, int n)
{
	t_list	*head;
	t_list	*node;

	head = NULL;
	while (n-- > 0)
	{
		node = malloc(sizeof(t_list));
		node->data = v[n];
		node->next = head;
		head = node;
	}
	return (head);
}

static void	show(t_list *l)
{
	while (l)
	{
		printf("%d ", l->data);
		l = l->next;
	}
	printf("\n");
}

int	main(void)
{
	int	a[] = {5, 4, 3, 2, 1};
	int	b[] = {1, 2, 3};
	int	c[] = {3, 1, 3, 1, 2};
	int	d[] = {42};

	show(sort_list(build(a, 5), ascending));
	show(sort_list(build(b, 3), ascending));
	show(sort_list(build(c, 5), ascending));
	show(sort_list(build(c, 5), descending));
	show(sort_list(build(d, 1), ascending));
	show(sort_list(NULL, ascending));
	return (0);
}
