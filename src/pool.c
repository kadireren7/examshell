/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pool.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:25:52 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 01:25:53 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "examshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
** The exercise pool. Levels mirror the archive laid out in level0..level5/,
** which is where the subjects are read from at runtime.
**
** `syms` lists the libc symbols the linker is allowed to resolve for that
** assignment; anything else undefined in the student's object files is
** treated as a forbidden function, exactly like the real moulinette does.
*/
static const t_exercise	g_pool[] = {
/* ---------------------------- level 0 ---------------------------------- */
{"aff_a", 0, 1, EX_PROGRAM, {"aff_a.c", NULL}, "write", {"write", NULL}},
{"aff_first_param", 0, 3, EX_PROGRAM, {"aff_first_param.c", NULL}, "write",
	{"write", NULL}},
{"aff_last_param", 0, 3, EX_PROGRAM, {"aff_last_param.c", NULL}, "write",
	{"write", NULL}},
{"aff_z", 0, 1, EX_PROGRAM, {"aff_z.c", NULL}, "write", {"write", NULL}},
{"maff_alpha", 0, 4, EX_PROGRAM, {"maff_alpha.c", NULL}, "write", {"write", NULL}},
{"maff_revalpha", 0, 4, EX_PROGRAM, {"maff_revalpha.c", NULL}, "write",
	{"write", NULL}},
{"only_a", 0, 2, EX_PROGRAM, {"only_a.c", NULL}, "write", {"write", NULL}},
{"only_z", 0, 2, EX_PROGRAM, {"only_z.c", NULL}, "write", {"write", NULL}},

/* ---------------------------- level 1 ---------------------------------- */
{"first_word", 1, 2, EX_PROGRAM, {"first_word.c", NULL}, "write",
	{"write", NULL}},
{"fizzbuzz", 1, 3, EX_PROGRAM, {"fizzbuzz.c", NULL}, "write", {"write", NULL}},
{"ft_countdown", 1, 1, EX_PROGRAM, {"ft_countdown.c", NULL}, "write",
	{"write", NULL}},
{"ft_print_numbers", 1, 1, EX_FUNCTION, {"ft_print_numbers.c", NULL}, "write",
	{"write", NULL}},
{"ft_putstr", 1, 2, EX_FUNCTION, {"ft_putstr.c", NULL}, "write", {"write", NULL}},
{"ft_strcpy", 1, 2, EX_FUNCTION, {"ft_strcpy.c", NULL}, "", {NULL}},
{"ft_strlen", 1, 1, EX_FUNCTION, {"ft_strlen.c", NULL}, "", {NULL}},
{"ft_swap", 1, 2, EX_FUNCTION, {"ft_swap.c", NULL}, "", {NULL}},
{"hello", 1, 1, EX_PROGRAM, {"hello.c", NULL}, "write", {"write", NULL}},
{"repeat_alpha", 1, 4, EX_PROGRAM, {"repeat_alpha.c", NULL}, "write",
	{"write", NULL}},
{"rev_print", 1, 3, EX_PROGRAM, {"rev_print.c", NULL}, "write", {"write", NULL}},
{"rot_13", 1, 3, EX_PROGRAM, {"rot_13.c", NULL}, "write", {"write", NULL}},
{"rotone", 1, 3, EX_PROGRAM, {"rotone.c", NULL}, "write", {"write", NULL}},
{"search_and_replace", 1, 4, EX_PROGRAM, {"search_and_replace.c", NULL},
	"write, exit", {"write", "exit", NULL}},
{"ulstr", 1, 4, EX_PROGRAM, {"ulstr.c", NULL}, "write", {"write", NULL}},

/* ---------------------------- level 2 ---------------------------------- */
{"alpha_mirror", 2, 2, EX_PROGRAM, {"alpha_mirror.c", NULL}, "write",
	{"write", NULL}},
{"do_op", 2, 3, EX_PROGRAM, {"*.c", NULL}, "atoi, printf, write",
	{"atoi", "printf", "puts", "putchar", "write", NULL}},
{"ft_atoi", 2, 3, EX_FUNCTION, {"ft_atoi.c", NULL}, "None", {NULL}},
{"ft_strcmp", 2, 1, EX_FUNCTION, {"ft_strcmp.c", NULL}, "", {NULL}},
{"ft_strdup", 2, 4, EX_FUNCTION, {"ft_strdup.c", NULL}, "malloc",
	{"malloc", NULL}},
{"ft_strrev", 2, 2, EX_FUNCTION, {"ft_strrev.c", NULL}, "", {NULL}},
{"inter", 2, 3, EX_PROGRAM, {"inter.c", NULL}, "write", {"write", NULL}},
{"is_power_of_2", 2, 1, EX_FUNCTION, {"is_power_of_2.c", NULL}, "None", {NULL}},
{"last_word", 2, 2, EX_PROGRAM, {"last_word.c", NULL}, "write", {"write", NULL}},
{"max", 2, 1, EX_FUNCTION, {"max.c", NULL}, "", {NULL}},
{"print_bits", 2, 2, EX_FUNCTION, {"print_bits.c", NULL}, "write",
	{"write", NULL}},
{"reverse_bits", 2, 1, EX_FUNCTION, {"reverse_bits.c", NULL}, "", {NULL}},
{"swap_bits", 2, 1, EX_FUNCTION, {"swap_bits.c", NULL}, "", {NULL}},
{"union", 2, 4, EX_PROGRAM, {"union.c", NULL}, "write", {"write", NULL}},
{"wdmatch", 2, 3, EX_PROGRAM, {"wdmatch.c", NULL}, "write", {"write", NULL}},

/* ---------------------------- level 3 ---------------------------------- */
{"add_prime_sum", 3, 3, EX_PROGRAM, {"add_prime_sum.c", NULL}, "write, exit",
	{"write", "exit", NULL}},
{"epur_str", 3, 3, EX_PROGRAM, {"epur_str.c", NULL}, "write", {"write", NULL}},
{"expand_str", 3, 3, EX_PROGRAM, {"expand_str.c", NULL}, "write",
	{"write", NULL}},
{"ft_atoi_base", 3, 4, EX_FUNCTION, {"ft_atoi_base.c", NULL}, "None", {NULL}},
{"ft_list_size", 3, 3, EX_FUNCTION, {"ft_list_size.c", "ft_list.h", NULL}, "",
	{NULL}},
{"ft_range", 3, 4, EX_FUNCTION, {"ft_range.c", NULL}, "malloc",
	{"malloc", NULL}},
{"ft_rrange", 3, 4, EX_FUNCTION, {"ft_rrange.c", NULL}, "malloc",
	{"malloc", NULL}},
{"hidenp", 3, 2, EX_PROGRAM, {"hidenp.c", NULL}, "write", {"write", NULL}},
{"lcm", 3, 2, EX_FUNCTION, {"lcm.c", NULL}, "", {NULL}},
{"paramsum", 3, 1, EX_PROGRAM, {"paramsum.c", NULL}, "write", {"write", NULL}},
{"pgcd", 3, 2, EX_PROGRAM, {"pgcd.c", NULL}, "printf, atoi, malloc, free",
	{"printf", "puts", "putchar", "atoi", "malloc", "free", NULL}},
{"print_hex", 3, 1, EX_PROGRAM, {"print_hex.c", NULL}, "write", {"write", NULL}},
{"rstr_capitalizer", 3, 4, EX_PROGRAM, {"rstr_capitalizer.c", NULL}, "write",
	{"write", NULL}},
{"str_capitalizer", 3, 4, EX_PROGRAM, {"str_capitalizer.c", NULL}, "write",
	{"write", NULL}},
{"tab_mult", 3, 1, EX_PROGRAM, {"tab_mult.c", NULL}, "write", {"write", NULL}},

/* ---------------------------- level 4 ---------------------------------- */
{"flood_fill", 4, 3, EX_FUNCTION, {"*.c", NULL}, "-", {NULL},
	{"t_point.h", NULL}},
{"fprime", 4, 2, EX_PROGRAM, {"fprime.c", NULL}, "printf, atoi",
	{"printf", "puts", "putchar", "atoi", NULL}},
{"ft_itoa", 4, 2, EX_FUNCTION, {"ft_itoa.c", NULL}, "malloc", {"malloc", NULL}},
{"ft_list_foreach", 4, 1, EX_FUNCTION, {"ft_list_foreach.c", "ft_list.h", NULL},
	"", {NULL}},
{"ft_list_remove_if", 4, 4, EX_FUNCTION, {"ft_list_remove_if.c", NULL}, "free",
	{"free", NULL}, {"ft_list.h", NULL}},
{"ft_split", 4, 4, EX_FUNCTION, {"ft_split.c", NULL}, "malloc",
	{"malloc", NULL}},
{"moment", 4, 2, EX_FUNCTION, {"moment.c", NULL}, "malloc, free",
	{"malloc", "free", NULL}},
{"rev_wstr", 4, 3, EX_PROGRAM, {"rev_wstr.c", NULL}, "write, malloc, free",
	{"write", "malloc", "free", NULL}},
{"rostring", 4, 3, EX_PROGRAM, {"rostring.c", NULL}, "write, malloc, free",
	{"write", "malloc", "free", NULL}},
{"sort_int_tab", 4, 1, EX_FUNCTION, {"sort_int_tab.c", NULL}, "", {NULL}},
{"sort_list", 4, 4, EX_FUNCTION, {"sort_list.c", NULL}, "", {NULL},
	{"list.h", NULL}},

/* ---------------------------- level 5 ---------------------------------- */
{"biggest_pal", 5, 4, EX_PROGRAM, {"*.c", NULL}, "write",
	{"write", NULL}},
{"brackets", 5, 2, EX_PROGRAM, {"*.c", NULL}, "write", {"write", NULL}},
{"brainfuck", 5, 4, EX_PROGRAM, {"*.c", NULL}, "write, malloc, free",
	{"write", "malloc", "free", NULL}},
{"check_mate", 5, 4, EX_PROGRAM, {"*.c", NULL}, "write, malloc, free",
	{"write", "malloc", "free", NULL}},
{"cycle_detector", 5, 3, EX_FUNCTION, {"cycle_detector.c", NULL}, "malloc, free",
	{"malloc", "free", NULL}, {"list.h", NULL}},
{"ft_itoa_base", 5, 2, EX_FUNCTION, {"ft_itoa_base.c", NULL}, "malloc",
	{"malloc", NULL}},
{"options", 5, 1, EX_PROGRAM, {"*.c", NULL}, "write", {"write", NULL}},
{"print_memory", 5, 1, EX_FUNCTION, {"print_memory.c", NULL}, "write",
	{"write", NULL}},
{"rpn_calc", 5, 3, EX_PROGRAM, {"*.c", NULL},
	"atoi, printf, write, malloc, free",
	{"atoi", "printf", "puts", "putchar", "write", "malloc", "free", NULL}},
};

const t_exercise	*pool_all(size_t *count)
{
	if (count)
		*count = sizeof(g_pool) / sizeof(g_pool[0]);
	return (g_pool);
}

const t_exercise	*pool_find(const char *name)
{
	size_t	i;
	size_t	n;
	char	path[MAX_PATH];

	pool_all(&n);
	i = 0;
	while (i < n)
	{
		if (!strcmp(g_pool[i].name, name))
		{
			snprintf(path, sizeof(path), "%s/level%d/%s/ref.c", es_root(),
				g_pool[i].level, g_pool[i].name);
			if (path_exists(path))
				return (&g_pool[i]);
			return (NULL);
		}
		i++;
	}
	return (NULL);
}

static int	already_used(const t_exercise *ex, const char **used, size_t used_n)
{
	size_t	i;

	i = 0;
	while (i < used_n)
	{
		if (used[i] && !strcmp(used[i], ex->name))
			return (1);
		i++;
	}
	return (0);
}

/*
** Pick a random unused exercise from `level`, preferring the requested
** difficulty. If that band is empty, keep the level and use the closest band.
** Only exercises that actually have a reference implementation on disk are
** eligible, so a half-populated checkout degrades gracefully instead of
** handing out ungradeable questions.
*/
int	pool_pick(int level, int difficulty, const char **used, size_t used_n,
		const t_exercise **out)
{
	const t_exercise	*cand[MAX_POOL];
	size_t				n;
	size_t				i;
	size_t				c;
	int					best;
	int					distance;
	char				path[MAX_PATH];

	if (difficulty < 1 || difficulty > 4)
		return (-1);
	pool_all(&n);
	c = 0;
	best = 5;
	i = 0;
	while (i < n)
	{
		snprintf(path, sizeof(path), "%s/level%d/%s/ref.c", es_root(),
			g_pool[i].level, g_pool[i].name);
		if (g_pool[i].level == level && !already_used(&g_pool[i], used, used_n)
			&& path_exists(path))
		{
			distance = abs(g_pool[i].difficulty - difficulty);
			if (distance < best)
			{
				best = distance;
				c = 0;
			}
			if (distance == best)
				cand[c++] = &g_pool[i];
		}
		i++;
	}
	if (c == 0)
		return (-1);
	*out = cand[(size_t)rand() % c];
	return (0);
}

char	*pool_subject(const t_exercise *ex)
{
	char	path[MAX_PATH];

	snprintf(path, sizeof(path), "%s/level%d/%s/subject.en.txt", es_root(),
		ex->level, ex->name);
	return (read_file(path));
}

/* The Turkish rendering of the subject, when one has been written. */
char	*pool_subject_tr(const t_exercise *ex)
{
	char	path[MAX_PATH];

	snprintf(path, sizeof(path), "%s/level%d/%s/subject.tr.txt", es_root(),
		ex->level, ex->name);
	return (read_file(path));
}

char	*pool_examples(const t_exercise *ex)
{
	char	path[MAX_PATH];

	snprintf(path, sizeof(path), "%s/level%d/%s/examples.txt", es_root(),
		ex->level, ex->name);
	return (read_file(path));
}

/*
** Install the headers the exam provides into `dir`. The subjects that come
** with one are explicit that the graded build uses ours, so this is called
** both when handing the question out and again just before compiling.
*/
int	pool_place_given(const t_exercise *ex, const char *dir)
{
	char	src[MAX_PATH];
	char	dst[MAX_PATH];
	int		i;

	i = 0;
	while (ex->given[i])
	{
		snprintf(src, sizeof(src), "%s/level%d/%s/given/%s", es_root(),
			ex->level, ex->name, ex->given[i]);
		snprintf(dst, sizeof(dst), "%s/%s", dir, ex->given[i]);
		if (copy_file(src, dst) != 0)
			return (-1);
		i++;
	}
	return (0);
}
