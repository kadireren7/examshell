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

#include <unistd.h>
static void	put(char c)
{
	write(1, &c, 1);
}

static void	puts_(const char *s)
{
	while (*s)
		put(*s++);
}

static int	balanced(char *s)
{
	char	stack[4096];
	int		top;
	int		i;

	top = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] == '(' || s[i] == '[' || s[i] == '{')
		{
			if (top >= 4096)
				return (0);
			stack[top++] = s[i];
		}
		else if (s[i] == ')' || s[i] == ']' || s[i] == '}')
		{
			char	want;

			if (s[i] == ')')
				want = '(';
			else if (s[i] == ']')
				want = '[';
			else
				want = '{';
			if (top == 0 || stack[top - 1] != want)
				return (0);
			top--;
		}
		i++;
	}
	return (top == 0);
}

int	main(int argc, char **argv)
{
	int	i;

	if (argc < 2)
	{
		put('\n');
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		puts_(balanced(argv[i]) ? "OK" : "Error");
		put('\n');
		i++;
	}
	return (0);
}
