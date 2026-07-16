/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:26:08 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 02:30:24 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "examshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void	ui_rule(void)
{
	printf(C_GREY "  " RULE C_RESET "\n");
}

/*
** Exam paths are long, deep and nearly all prefix. Folding $HOME back to "~"
** keeps them short enough to read at a glance and still paste into an editor.
*/
const char	*ui_short(const char *path, char *out, size_t n)
{
	const char	*home;
	size_t		len;

	home = getenv("HOME");
	if (!home || !*home)
		return (path);
	len = strlen(home);
	if (strncmp(path, home, len) != 0 || (path[len] && path[len] != '/'))
		return (path);
	snprintf(out, n, "~%s", path + len);
	return (out);
}

void	ui_banner(void)
{
	printf("\n" C_CYAN C_BOLD);
	printf("   ___                     ___ _        _ _\n");
	printf("  | __|_ ____ _ _ __      / __| |_  ___| | |\n");
	printf("  | _|\\ \\ / _` | '  \\    _\\__ \\ ' \\/ -_) | |\n");
	printf("  |___/_\\_\\__,_|_|_|_|  (_)___/_||_\\___|_|_|\n");
	printf(C_RESET);
	printf(C_GREY "\n  42 Piscine exam simulator " C_RESET C_DIM "v" ES_VERSION
		C_RESET "\n\n");
}

void	ui_menu(void)
{
	const t_exam	*e;
	size_t			n;
	size_t			i;
	char			t[32];

	e = exam_all(&n);
	printf(C_BOLD "  Choose your exam:" C_RESET "\n\n");
	i = 0;
	while (i < n)
	{
		ui_fmt_time(e[i].duration, t, sizeof(t));
		printf("   " C_YELLOW "[%zu]" C_RESET " " C_BOLD "%-11s" C_RESET
			C_GREY "%2zu questions  %s" C_RESET "\n", i + 1, e[i].label,
			e[i].nslots, t);
		printf("       " C_DIM "%s" C_RESET "\n", e[i].blurb);
		i++;
	}
	printf("\n   " C_YELLOW "[q]" C_RESET " Quit\n\n");
}

void	ui_fmt_time(int seconds, char *out, size_t n)
{
	if (seconds >= 3600)
		snprintf(out, n, "%dh%02dm", seconds / 3600, (seconds % 3600) / 60);
	else if (seconds >= 60)
		snprintf(out, n, "%dm%02ds", seconds / 60, seconds % 60);
	else
		snprintf(out, n, "%ds", seconds);
}

/*
** What the student is shown on arrival: where the exam lives, and how to hand
** it in. Everything sits under one directory, so it is named once and the two
** that matter hang off it -- three copies of the same long path told the
** student nothing the first one did not.
*/
void	ui_exam_header(const t_session *s)
{
	char	t[32];
	char	buf[MAX_PATH];

	ui_fmt_time(s->exam->duration, t, sizeof(t));
	printf("\n  " C_CYAN C_BOLD "%s" C_RESET C_GREY "   %zu questions   %s"
		"   pass %d/100" C_RESET "\n\n", s->exam->label, s->exam->nslots, t,
		s->exam->pass);
	printf("  " C_BOLD "workspace" C_RESET "  %s\n",
		ui_short(s->workspace, buf, sizeof(buf)));
	printf("    " C_GREEN "rendu/" C_RESET C_DIM "   your code, a clone of the"
		" exam server -- commit and push here" C_RESET "\n");
	printf("    " C_CYAN "exam/" C_RESET C_DIM "    subject.txt,"
		" subject.tr.txt, traces/" C_RESET "\n");
	printf("    " C_YELLOW "origin" C_RESET "   " C_DIM "%s" C_RESET "   %s\n",
		s->url, repo_link_ok(s) ? C_GREEN "linked" C_RESET
		: C_RED "BROKEN" C_RESET);
	printf("\n  " C_BOLD "submit" C_RESET "     " C_DIM "cd rendu" C_GREY
		"  ->  " C_RESET C_DIM "git add . && git commit -m x" C_GREY "  ->  "
		C_RESET C_DIM "git push" C_RESET C_GREY "  ->  " C_RESET C_BOLD
		"grademe" C_RESET "\n");
	printf("  " C_BOLD "commands" C_RESET "   " C_CYAN "grademe" C_RESET
		C_DIM " grades what you pushed, never your working copy" C_RESET "\n");
	printf("             " C_CYAN "logs" C_RESET C_DIM "    shows every error and"
		" test the robot tried" C_RESET "\n");
	printf("             " C_CYAN "finish" C_RESET C_DIM "  ends the exam and"
		" gives you your grade" C_RESET "\n");
}

/*
** Announce a question. The subject's text is never printed: the student is
** told which assignment it is and which file to read it from.
*/
void	ui_question(const t_session *s)
{
	t_question	*q;
	char		t[32];
	char		path[MAX_PATH * 2];
	char		buf[MAX_PATH];

	q = session_current((t_session *)s);
	if (!q)
		return ;
	ui_fmt_time(session_remaining(s), t, sizeof(t));
	printf("\n");
	ui_rule();
	printf("  " C_BOLD "Question %zu/%zu" C_RESET "   " C_MAGENTA "level %d"
		C_RESET C_GREY "  difficulty %d/4" C_RESET "   " C_YELLOW "%d points"
		C_RESET "   " C_GREY "%s left" C_RESET "\n\n", s->cur + 1, s->nq,
		q->ex->level, q->ex->difficulty, q->points, t);
	printf("    " C_BOLD C_CYAN "%s" C_RESET C_GREY "   turn in   " C_RESET
		"%s\n", q->ex->name, *q->ex->files[0] == '*' ? "*.c, *.h"
		: q->ex->files[0]);
	snprintf(path, sizeof(path), "%s/subject.txt", s->docs);
	printf("    " C_GREY "subject   " C_RESET "%s" C_GREY
		"   (turkce: subject.tr.txt)" C_RESET "\n",
		ui_short(path, buf, sizeof(buf)));
	snprintf(path, sizeof(path), "%s/examples.txt", s->docs);
	if (path_exists(path))
		printf("    " C_GREY "examples  " C_RESET "%s\n",
			ui_short(path, buf, sizeof(buf)));
	printf("\n");
}

/*
** The failure report. It shows every case that was run and which of them
** broke, plus the offending line of the submission's own output -- and never
** the expected value. Knowing that "./rot_13 \"AkjhZ zLKIJz , 23y \"" is
** wrong is a test result; being told what it should have printed would be
** the answer.
*/
static void	ui_test_log(const t_report *rep, const t_question *q)
{
	int	i;

	if (rep->ncases <= 0)
		return ;
	printf("\n " C_BOLD "test log" C_RESET C_GREY "  (%d/%d passed)" C_RESET
		"\n\n", rep->passed, rep->total);
	i = 0;
	while (i < rep->ncases)
	{
		const t_case_result	*c;

		c = &rep->cases[i];
		if (c->verdict == V_OK)
			printf("   " C_GREEN "[ok]" C_RESET C_GREY " %2d  ./%s %s"
				C_RESET "\n", i + 1, q->ex->name, c->args);
		else
		{
			printf("   " C_RED "[KO]" C_RESET " %2d  " C_BOLD "./%s %s"
				C_RESET "\n", i + 1, q->ex->name, c->args);
			if (c->verdict == V_WRONG)
				printf("        " C_GREY "your output, line %d:" C_RESET
					" %s\n", c->diff_line, c->got);
			else
				printf("        " C_RED "%s" C_RESET "\n",
					verdict_str(c->verdict));
		}
		i++;
	}
}

void	ui_report(const t_session *s, const t_report *rep)
{
	t_question	*q;

	q = session_current((t_session *)s);
	printf("\n");
	if (rep->verdict == V_OK)
	{
		printf(" " C_GREEN C_BOLD "Success!" C_RESET " %s: " C_GREY
			"%d/%d tests" C_RESET "  " C_YELLOW "+%d points" C_RESET "\n",
			q ? q->ex->name : "", rep->passed, rep->total,
			q ? q->points : 0);
		printf(" " C_GREY "grade: %d/100" C_RESET "\n\n", s->grade);
		return ;
	}
	printf(" " C_RED C_BOLD "Failure." C_RESET " %s: " C_RED "%s" C_RESET "\n",
		q ? q->ex->name : "", verdict_str(rep->verdict));
	if (rep->detail[0])
		printf("\n%s\n", rep->detail);
	if (q)
		ui_test_log(rep, q);
	if (rep->trace[0])
	{
		char	buf[MAX_PATH];

		printf("\n " C_GREY "trace: %s" C_RESET "\n",
			ui_short(rep->trace, buf, sizeof(buf)));
	}
	printf("\n " C_GREY "No points lost. Fix it and run grademe again."
		C_RESET "\n\n");
}

void	ui_final(const t_session *s)
{
	size_t	i;
	int		elapsed;
	char	t[32];
	char	buf[MAX_PATH];

	elapsed = (int)(time(NULL) - s->start);
	if (elapsed > s->exam->duration)
		elapsed = s->exam->duration;
	ui_fmt_time(elapsed, t, sizeof(t));
	printf("\n");
	ui_rule();
	printf(" " C_BOLD "%s -- results" C_RESET "\n", s->exam->label);
	ui_rule();
	i = 0;
	while (i < s->nq)
	{
		printf("  %s%-4s" C_RESET "%-20s " C_GREY "level %d  difficulty"
			" %d/4" C_RESET "   %s%+3d" C_RESET "\n",
			s->q[i].state == Q_PASSED ? C_GREEN : C_RED,
			s->q[i].state == Q_PASSED ? "OK" : "KO", s->q[i].ex->name,
			s->q[i].ex->level, s->q[i].ex->difficulty,
			s->q[i].state == Q_PASSED ? C_GREEN : C_GREY,
			s->q[i].state == Q_PASSED ? s->q[i].points : 0);
		i++;
	}
	ui_rule();
	printf("  time spent : %s\n", t);
	printf("  " C_BOLD "grade      : %s%d/100" C_RESET "\n",
		s->grade >= s->exam->pass ? C_GREEN : C_RED, s->grade);
	printf("  %s\n", s->grade >= s->exam->pass
		? C_GREEN C_BOLD "EXAM VALIDATED" C_RESET
		: C_RED C_BOLD "EXAM FAILED" C_RESET);
	ui_rule();
	printf(" " C_GREY "Your work stayed behind in %s" C_RESET "\n\n",
		ui_short(s->workspace, buf, sizeof(buf)));
}
