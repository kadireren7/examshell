/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:25:48 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 01:25:49 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "examshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

static void	trim(char *s)
{
	size_t	n;

	n = strlen(s);
	while (n && (s[n - 1] == '\n' || s[n - 1] == ' ' || s[n - 1] == '\t'))
		s[--n] = '\0';
}

static const t_exam	*choose(void)
{
	char	line[64];
	size_t	n;
	long	pick;

	exam_all(&n);
	while (1)
	{
		printf("  " C_BOLD "select" C_RESET "$> ");
		fflush(stdout);
		if (!fgets(line, sizeof(line), stdin))
			return (NULL);
		trim(line);
		if (!strcmp(line, "q") || !strcmp(line, "quit")
			|| !strcmp(line, "exit"))
			return (NULL);
		{
			const t_exam	*e;
			size_t			i;

			e = exam_all(&n);
			i = 0;
			while (i < n)
			{
				if (!strcmp(line, e[i].id))
					return (&e[i]);
				i++;
			}
		}
		pick = strtol(line, NULL, 10);
		if (pick >= 1 && (size_t)pick <= n)
			return (exam_by_index((size_t)pick - 1));
		printf(C_RED "  pick 1-%zu, an exam id, or q to quit." C_RESET "\n", n);
	}
}

static int	preflight(void)
{
	char	path[MAX_PATH];

	snprintf(path, sizeof(path), "%s/level0/aff_a/ref.c", es_root());
	if (!path_exists(path))
	{
		fprintf(stderr, C_RED "examshell: cannot find the exercise data at %s"
			C_RESET "\n", path);
		fprintf(stderr, "Run examshell from the repository, or set"
			" EXAMSHELL_ROOT to point at it.\n");
		return (-1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_session		s;
	const t_exam	*exam;

	signal(SIGPIPE, SIG_IGN);
	setvbuf(stdout, NULL, _IOLBF, 0);
	if (preflight() != 0)
		return (1);
	if (argc > 1 && !strcmp(argv[1], "--selftest"))
		return (selftest(argc > 2 ? atoi(argv[2]) : -1));
	ui_banner();
	exam = NULL;
	if (argc > 1)
	{
		size_t			n;
		const t_exam	*e;
		size_t			i;

		e = exam_all(&n);
		i = 0;
		while (i < n)
		{
			if (!strcmp(argv[1], e[i].id))
				exam = &e[i];
			i++;
		}
		if (!exam)
		{
			fprintf(stderr, C_RED "examshell: unknown exam '%s'" C_RESET "\n",
				argv[1]);
			return (1);
		}
	}
	else
	{
		ui_menu();
		exam = choose();
	}
	if (!exam)
	{
		printf("  Bye.\n");
		return (0);
	}
	if (session_start(&s, exam) != 0)
		return (1);
	return (shell_loop(&s));
}
