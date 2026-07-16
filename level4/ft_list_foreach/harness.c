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

void	ft_list_foreach(t_list *begin_list, void (*f)(void *));

static void	show(void *p)
{
	printf("[%s]", (char *)p);
}

static void	count(void *p)
{
	(void)p;
	printf(".");
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

int	main(void)
{
	char	*w[] = {"one", "two", "three"};

	ft_list_foreach(build(w, 3), show);
	printf("\n");
	ft_list_foreach(build(w, 3), count);
	printf("\n");
	ft_list_foreach(build(w, 1), show);
	printf("\n");
	ft_list_foreach(NULL, show);
	printf("empty ok\n");
	return (0);
}
