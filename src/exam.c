/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exam.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:25:43 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 06:15:18 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "examshell.h"
#include <stddef.h>

/*
** Exam definitions.
**
** exam00/01/02 : 10 questions, 10 points each -> 100. All start at level 0
**                and rise from easy to hard.
** final        : 14 questions. Levels 0-3 are worth 6 points (10 questions,
**                60 points), levels 4-5 are worth 10 points (4 questions,
**                40 points) -> 100.
**
** A failed submission costs no points: you keep the question and the only
** thing you spend is time. Each exam walks its levels from easy to hard.
** Within each level, Exam 00 asks for difficulty 1, Exam 01 for 2, Exam 02
** for 3, and the Final for 4. pool_pick() uses the closest available rank
** when a level has exhausted the exact one. A slot never falls through to a
** different level, and every exam plan is validated to begin at level 0.
*/
static const t_exam	g_exams[] = {
{
	"exam00", "Exam 00",
	"Functions, loops, conditionals, arrays and strings.",
	4 * 3600, 30, 10,
	{
		{0, 10, 1}, {0, 10, 1}, {1, 10, 1}, {1, 10, 1}, {1, 10, 1},
		{2, 10, 1}, {2, 10, 1}, {2, 10, 1}, {3, 10, 1}, {3, 10, 1}
	}
},
{
	"exam01", "Exam 01",
	"Strings, pointers, memory and bitwise operations.",
	4 * 3600, 30, 10,
	{
		{0, 10, 2}, {0, 10, 2}, {1, 10, 2}, {1, 10, 2}, {2, 10, 2},
		{2, 10, 2}, {3, 10, 2}, {3, 10, 2}, {4, 10, 2}, {4, 10, 2}
	}
},
{
	"exam02", "Exam 02",
	"Allocation, linked lists, function pointers and algorithms.",
	4 * 3600, 30, 10,
	{
		{0, 10, 3}, {0, 10, 3}, {1, 10, 3}, {1, 10, 3}, {2, 10, 3},
		{2, 10, 3}, {3, 10, 3}, {4, 10, 3}, {5, 10, 3}, {5, 10, 3}
	}
},
{
	"final", "Final Exam",
	"Everything above, twice the time and no mercy.",
	8 * 3600, 30, 14,
	{
		{0, 6, 4}, {0, 6, 4}, {1, 6, 4}, {1, 6, 4},
		{2, 6, 4, "ft_atoi"}, {2, 6, 4}, {2, 6, 4},
		{3, 6, 4}, {3, 6, 4}, {3, 6, 4},
		{4, 10, 4}, {4, 10, 4}, {5, 10, 4}, {5, 10, 4}
	}
},
};

const t_exam	*exam_all(size_t *count)
{
	if (count)
		*count = sizeof(g_exams) / sizeof(g_exams[0]);
	return (g_exams);
}

const t_exam	*exam_by_index(size_t i)
{
	size_t	n;

	exam_all(&n);
	if (i >= n)
		return (NULL);
	return (&g_exams[i]);
}
