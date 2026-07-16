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
#include <string.h>

typedef struct	s_list
{
	struct s_list	*next;
	void			*data;
}				t_list;

void	ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)());

static int	cmp(void *a, void *b)
{
	return (strcmp((char *)a, (char *)b));
}

static t_list	*build(char **words, int n)
{
	t_list	*head;
	t_list	*node;

	head = NULL;
	while (n-- > 0)
	{
		node = malloc(sizeof(t_list));
		node->data = words[n];
		node->next = head;
		head = node;
	}
	return (head);
}

static void	show(t_list *l)
{
	while (l)
	{
		printf("[%s]", (char *)l->data);
		l = l->next;
	}
	printf("\n");
}

int	main(void)
{
	char	*a[] = {"a", "b", "a", "c", "a"};
	char	*b[] = {"a", "a", "a"};
	char	*c[] = {"x", "y", "z"};
	t_list	*l;

	l = build(a, 5);
	ft_list_remove_if(&l, "a", cmp);
	show(l);
	l = build(b, 3);
	ft_list_remove_if(&l, "a", cmp);
	show(l);
	l = build(c, 3);
	ft_list_remove_if(&l, "a", cmp);
	show(l);
	l = build(c, 3);
	ft_list_remove_if(&l, "z", cmp);
	show(l);
	l = NULL;
	ft_list_remove_if(&l, "a", cmp);
	printf("empty ok\n");
	return (0);
}
