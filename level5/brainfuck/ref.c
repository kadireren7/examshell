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

static int	match_forward(char *code, int pc)
{
	int	depth;

	depth = 1;
	while (code[++pc])
	{
		if (code[pc] == '[')
			depth++;
		else if (code[pc] == ']' && --depth == 0)
			return (pc);
	}
	return (pc);
}

static int	match_back(char *code, int pc)
{
	int	depth;

	depth = 1;
	while (--pc >= 0)
	{
		if (code[pc] == ']')
			depth++;
		else if (code[pc] == '[' && --depth == 0)
			return (pc);
	}
	return (pc);
}

int	main(int argc, char **argv)
{
	unsigned char	tape[2048];
	int				p;
	int				pc;
	char			*code;

	if (argc != 2)
	{
		put('\n');
		return (0);
	}
	p = 0;
	pc = 0;
	code = argv[1];
	while (p < 2048)
		tape[p++] = 0;
	p = 0;
	while (code[pc])
	{
		if (code[pc] == '>')
			p++;
		else if (code[pc] == '<')
			p--;
		else if (code[pc] == '+')
			tape[p]++;
		else if (code[pc] == '-')
			tape[p]--;
		else if (code[pc] == '.')
			put((char)tape[p]);
		else if (code[pc] == '[' && !tape[p])
			pc = match_forward(code, pc);
		else if (code[pc] == ']' && tape[p])
			pc = match_back(code, pc);
		pc++;
	}
	return (0);
}
