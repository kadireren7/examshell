/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   session.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:26:00 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 06:15:18 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "examshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

t_question	*session_current(t_session *s)
{
	if (s->finished || s->cur >= s->nq)
		return (NULL);
	return (&s->q[s->cur]);
}

int	session_remaining(const t_session *s)
{
	int	left;

	left = (int)(s->deadline - time(NULL));
	if (left < 0)
		return (0);
	return (left);
}

/*
** Seed from something that actually differs run to run. time() alone repeats
** for every session started within the same second, which is exactly what
** happens when you restart the shell twice in a row.
*/
static void	seed_random(void)
{
	unsigned int	seed;
	FILE			*f;

	seed = (unsigned int)time(NULL) ^ ((unsigned int)getpid() << 8);
	f = fopen("/dev/urandom", "rb");
	if (f)
	{
		unsigned int	r;

		if (fread(&r, sizeof(r), 1, f) == 1)
			seed ^= r;
		fclose(f);
	}
	srand(seed);
}

/*
** Pick one assignment for a slot. It must not already be in this exam, and
** ideally it has not shown up in recent sessions either. The slot's level is
** strict: when recent history exhausts that level, drop the history preference
** and pick another unused assignment from the same level.
*/
static int	draw_one(int lvl, int difficulty, const char **avoid,
		size_t avoid_n, const char **used, size_t used_n,
		const t_exercise **ex)
{
	if (pool_pick(lvl, difficulty, avoid, avoid_n, ex) == 0)
		return (0);
	if (pool_pick(lvl, difficulty, used, used_n, ex) == 0)
		return (0);
	return (-1);
}

static int	exam_plan_valid(const t_exam *exam)
{
	size_t	i;
	int		previous_level;

	if (!exam || exam->nslots == 0 || exam->nslots > MAX_QUESTIONS
		|| exam->slots[0].level != 0)
		return (0);
	previous_level = 0;
	i = 0;
	while (i < exam->nslots)
	{
		if (exam->slots[i].level < previous_level
			|| exam->slots[i].level < 0 || exam->slots[i].level > 5
			|| exam->slots[i].points <= 0
			|| exam->slots[i].difficulty < 1
			|| exam->slots[i].difficulty > 4)
			return (0);
		previous_level = exam->slots[i].level;
		i++;
	}
	return (1);
}

int	session_plan_questions(t_session *s, const t_history *h)
{
	const char	*used[MAX_QUESTIONS];
	const char	*avoid[MAX_QUESTIONS + MAX_HISTORY];
	size_t		i;
	size_t		avoid_n;

	if (!s || !h || !exam_plan_valid(s->exam))
		return (-1);
	avoid_n = 0;
	while (avoid_n < h->n)
	{
		avoid[avoid_n] = h->name[avoid_n];
		avoid_n++;
	}
	i = 0;
	while (i < s->exam->nslots)
	{
		const t_exercise	*ex;

		avoid[avoid_n] = NULL;
		if (s->exam->slots[i].exercise)
		{
			ex = pool_find(s->exam->slots[i].exercise);
			if (!ex || ex->level != s->exam->slots[i].level)
				return (-1);
		}
		else if (draw_one(s->exam->slots[i].level,
				s->exam->slots[i].difficulty, avoid, avoid_n, used, i,
				&ex) != 0)
			return (-1);
		s->q[i].ex = ex;
		s->q[i].points = s->exam->slots[i].points;
		s->q[i].attempts = 0;
		s->q[i].state = Q_PENDING;
		used[i] = ex->name;
		avoid[avoid_n++] = ex->name;
		i++;
	}
	s->nq = s->exam->nslots;
	return (0);
}

int	session_start(t_session *s, const t_exam *exam)
{
	t_history	h;

	memset(s, 0, sizeof(*s));
	if (!exam_plan_valid(exam))
	{
		fprintf(stderr, C_RED "examshell: invalid exam plan" C_RESET "\n");
		return (-1);
	}
	s->exam = exam;
	seed_random();
	if (repo_plan(s) != 0)
		return (-1);
	history_load(&h);
	if (session_plan_questions(s, &h) != 0)
	{
		fprintf(stderr, C_RED "examshell: exercise pool is too small for %s"
			C_RESET "\n", exam->id);
		return (-1);
	}
	history_save(&h, s);
	if (repo_setup(s) != 0)
	{
		fprintf(stderr, C_RED "examshell: could not create the exam repository"
			C_RESET "\n");
		return (-1);
	}
	s->start = time(NULL);
	s->deadline = s->start + exam->duration;
	s->cur = 0;
	s->grade = 0;
	repo_arm(s);
	return (0);
}

/*
** Move to the next question and hand the student a clean repository. The
** previous assignment's files are wiped from the working copy and from the
** remote, so every question starts from an empty rendu, like the real exam.
*/
void	session_next(t_session *s)
{
	s->cur++;
	if (s->cur >= s->nq)
	{
		session_finish(s, 0);
		return ;
	}
	repo_reset(s);
	repo_arm(s);
}

void	session_finish(t_session *s, int by_user)
{
	size_t	i;

	(void)by_user;
	if (s->finished)
		return ;
	s->finished = 1;
	i = s->cur;
	while (i < s->nq)
	{
		if (s->q[i].state == Q_PENDING)
			s->q[i].state = Q_SKIPPED;
		i++;
	}
}
