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

#include <stdio.h>
#include <stdlib.h>

static int	is_op(char *t)
{
	return (t[1] == 0 && (t[0] == '+' || t[0] == '-' || t[0] == '*'
			|| t[0] == '/' || t[0] == '%'));
}

static int	is_num(char *t)
{
	int	i;

	i = 0;
	if (t[0] == '-' && t[1])
		i = 1;
	if (!t[i])
		return (0);
	while (t[i])
	{
		if (t[i] < '0' || t[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/* strtok(3) is not on this subject's list, so the split is done by hand. */
static char	*next_token(char *s, char **save)
{
	char	*start;

	if (s)
		*save = s;
	while (**save == ' ')
		(*save)++;
	if (!**save)
		return (0);
	start = *save;
	while (**save && **save != ' ')
		(*save)++;
	if (**save)
		*(*save)++ = '\0';
	return (start);
}

/* exit(3) is not on this subject's list either, so failure travels back. */
static int	eval(char *expr, long *out)
{
	long	stack[4096];
	int		top;
	char	*t;
	char	*save;

	top = 0;
	t = next_token(expr, &save);
	while (t)
	{
		if (is_num(t))
		{
			if (top >= 4096)
				return (-1);
			stack[top++] = atoi(t);
		}
		else if (is_op(t))
		{
			long	a;
			long	b;

			if (top < 2)
				return (-1);
			b = stack[--top];
			a = stack[--top];
			if ((t[0] == '/' || t[0] == '%') && b == 0)
				return (-1);
			if (t[0] == '+')
				stack[top++] = a + b;
			else if (t[0] == '-')
				stack[top++] = a - b;
			else if (t[0] == '*')
				stack[top++] = a * b;
			else if (t[0] == '/')
				stack[top++] = a / b;
			else
				stack[top++] = a % b;
		}
		else
			return (-1);
		t = next_token(NULL, &save);
	}
	if (top != 1)
		return (-1);
	*out = stack[0];
	return (0);
}

int	main(int argc, char **argv)
{
	long	res;

	if (argc != 2 || eval(argv[1], &res) != 0)
	{
		printf("Error\n");
		return (0);
	}
	printf("%ld\n", res);
	return (0);
}
