/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:26:03 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 02:30:24 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "examshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

/* Take a command however it was typed: padded, indented, or shouted at. */
static char	*trim(char *s)
{
	size_t	n;

	while (*s == ' ' || *s == '\t')
		s++;
	n = strlen(s);
	while (n && (s[n - 1] == '\n' || s[n - 1] == '\r' || s[n - 1] == ' '
			|| s[n - 1] == '\t'))
		s[--n] = '\0';
	return (s);
}

static void	prompt(const t_session *s)
{
	char	t[32];

	ui_fmt_time(session_remaining(s), t, sizeof(t));
	printf(C_GREEN C_BOLD "examshell" C_RESET C_GREY "[%s|%d/100]" C_RESET
		"$> ", t, s->grade);
	fflush(stdout);
}

/*
** A line too long for the buffer would otherwise come back in pieces, each
** piece read as a command of its own. Swallow the rest of it instead.
*/
static void	drain_line(const char *line)
{
	int	c;

	if (strchr(line, '\n'))
		return ;
	c = getchar();
	while (c != '\n' && c != EOF)
		c = getchar();
}

static void	cmd_grademe(t_session *s)
{
	t_question	*q;
	t_report	rep;
	int			guarded;
	int			restored;

	q = session_current(s);
	if (!q)
		return ;
	q->attempts++;
	{
		t_pushed	p;

		if (repo_pushed(s, &p))
			printf("\n " C_GREY "grading" C_RESET " " C_BOLD "%s" C_RESET
				C_GREY " from %s \"%s\" (%d file%s) ..." C_RESET "\n",
				q->ex->name, p.sha, p.subject, p.files,
				p.files > 1 ? "s" : "");
		else
			printf("\n " C_GREY "grading %s ..." C_RESET "\n", q->ex->name);
	}
	fflush(stdout);
	guarded = (repo_backup_sources(s) == 0);
	moulinette_run(s, q, &rep);
	restored = guarded ? repo_restore_sources(s) : 0;
	if (restored > 0)
		printf(" " C_YELLOW "safety:" C_RESET " restored %d source file%s"
			" changed during grading.\n", restored, restored > 1 ? "s" : "");
	if (rep.verdict == V_OK)
	{
		q->state = Q_PASSED;
		s->grade += q->points;
	}
	ui_report(s, &rep);
	if (rep.verdict != V_OK)
		return ;
	session_next(s);
	if (!s->finished)
		ui_question(s);
}

static void	print_trace(const char *path, int *found)
{
	char	*body;
	size_t	len;

	body = read_file(path);
	if (!body)
		return ;
	if (*found)
		printf("\n");
	printf(" " C_CYAN RULE C_RESET "\n%s", body);
	len = strlen(body);
	if (!len || body[len - 1] != '\n')
		printf("\n");
	*found = 1;
	free(body);
}

/* Show every failed grading attempt made during this exam. */
static void	cmd_logs(const t_session *s)
{
	char	path[MAX_PATH * 2];
	size_t	i;
	int		attempt;
	int		found;

	printf("\n " C_BOLD "grading history" C_RESET "\n");
	found = 0;
	i = 0;
	while (i < s->nq)
	{
		attempt = 1;
		while (attempt <= s->q[i].attempts)
		{
			snprintf(path, sizeof(path), "%s/traces/%s_attempt%d.trace",
				s->docs, s->q[i].ex->name, attempt);
			print_trace(path, &found);
			attempt++;
		}
		i++;
	}
	if (!found)
		printf(" " C_GREY "No failed grading attempts recorded yet."
			C_RESET "\n");
	printf("\n");
}

/*
** Keep the exam shell deliberately small: grade, inspect grading history, or
** stop. A stray git command typed into the wrong window gets pointed back at
** those commands.
*/
static int	dispatch(t_session *s, const char *line)
{
	if (!*line)
		return (0);
	if (!strcmp(line, "grademe"))
	{
		cmd_grademe(s);
		return (0);
	}
	if (!strcmp(line, "logs") || !strcmp(line, "history"))
	{
		cmd_logs(s);
		return (0);
	}
	if (!strcmp(line, "finish"))
	{
		session_finish(s, 1);
		return (1);
	}
	printf(" " C_RED "%s" C_RESET ": not a command. Use " C_CYAN "grademe"
		C_RESET ", " C_CYAN "logs" C_RESET " or " C_CYAN "finish"
		C_RESET ".\n", line);
	return (0);
}

/*
** ^C is not a way out of an exam. Interrupting the shell drops the line being
** typed and hands back a fresh prompt; the exam ends when the time does, or
** when the student says finish.
**
** The handler is installed without SA_RESTART on purpose: the point is for
** the pending fgets() to come back with EINTR so the loop can reprompt,
** which is exactly what a restarted read would not do.
*/
static void	on_interrupt(int sig)
{
	(void)sig;
}

static void	catch_interrupt(void)
{
	struct sigaction	sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = on_interrupt;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

int	shell_loop(t_session *s)
{
	char	line[512];

	catch_interrupt();
	ui_exam_header(s);
	ui_question(s);
	while (!s->finished)
	{
		prompt(s);
		if (!fgets(line, sizeof(line), stdin))
		{
			if (ferror(stdin) && errno == EINTR)
			{
				clearerr(stdin);
				printf("\n");
				continue ;
			}
			printf("\n");
			session_finish(s, 1);
			break ;
		}
		drain_line(line);
		if (session_remaining(s) == 0)
		{
			printf(C_RED "\n Time is up." C_RESET "\n");
			session_finish(s, 0);
			break ;
		}
		if (dispatch(s, trim(line)))
			break ;
	}
	ui_final(s);
	return (0);
}
