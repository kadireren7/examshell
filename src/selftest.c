/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selftest.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:25:58 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 06:18:09 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "examshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/*
** `examshell --selftest` is the robot. It sits the exam itself.
**
** For every assignment in the pool it stages an answer in rendu/, commits it,
** pushes it to the exam server and hands the push to the moulinette -- the
** same road a student walks, no shortcut into the grader.
**
** The answer it turns in is the reference implementation, so a correct
** examshell can only ever answer OK. A KO means the simulator is broken and
** the assignment is unpassable for everyone: a header the grader never puts
** where the compiler looks, an allow-list that forbids what the subject
** permits, a test case the reference itself cannot satisfy.
**
** The scenarios at the end come at it from the other side: they turn in work
** that is wrong on purpose and insist the moulinette notices, says why, and
** leaves a trace on disk.
*/

typedef struct s_tally
{
	int	ok;
	int	ko;
}	t_tally;

static void	tdir_of(const t_exercise *ex, char *out, size_t n)
{
	snprintf(out, n, "%s/level%d/%s", es_root(), ex->level, ex->name);
}

/*
** The file the reference implementation is turned in as. Subjects that ask
** for "*.c" take any name, so the assignment's own is the natural one.
*/
static void	answer_name(const t_exercise *ex, char *out, size_t n)
{
	int	i;

	if (strcmp(ex->files[0], "*.c") != 0)
	{
		i = 0;
		while (ex->files[i])
		{
			size_t	len = strlen(ex->files[i]);

			if (len > 2 && !strcmp(ex->files[i] + len - 2, ".c"))
			{
				snprintf(out, n, "%s", ex->files[i]);
				return ;
			}
			i++;
		}
		snprintf(out, n, "%s", ex->files[0]);
		return ;
	}
	snprintf(out, n, "%s.c", ex->name);
}

/*
** Stage everything the subject asks the student for: the answer itself, plus
** any header the subject says to turn in -- those live beside the reference
** in turnin/, since the exam does not hand them out.
*/
static int	stage_answer(t_session *s, const t_exercise *ex, char *why,
		size_t why_n)
{
	char	tdir[MAX_PATH];
	char	src[MAX_PATH * 2];
	char	dst[MAX_PATH * 2];
	char	name[MAX_PATH];
	int		i;

	tdir_of(ex, tdir, sizeof(tdir));
	answer_name(ex, name, sizeof(name));
	snprintf(src, sizeof(src), "%s/ref.c", tdir);
	snprintf(dst, sizeof(dst), "%s/%s", s->repo, name);
	if (copy_file(src, dst) != 0)
	{
		snprintf(why, why_n, "no reference implementation at %s", src);
		return (-1);
	}
	pool_place_given(ex, s->repo);
	i = 0;
	while (ex->files[i])
	{
		size_t	len = strlen(ex->files[i]);

		if (len > 2 && !strcmp(ex->files[i] + len - 2, ".h"))
		{
			snprintf(src, sizeof(src), "%s/turnin/%s", tdir, ex->files[i]);
			snprintf(dst, sizeof(dst), "%s/%s", s->repo, ex->files[i]);
			if (!path_exists(dst) && copy_file(src, dst) != 0)
			{
				snprintf(why, why_n, "the subject asks for %s but nothing"
					" hands the robot one (expected %s)", ex->files[i], src);
				return (-1);
			}
		}
		i++;
	}
	return (0);
}

static int	push(t_session *s, const char *msg)
{
	char	*add[] = {"git", "add", "-A", "-f", NULL};
	char	*ci[] = {"git", "commit", "--quiet", "--allow-empty", "-m",
		(char *)msg, NULL};
	char	*pu[] = {"git", "push", "--quiet", "--force", "origin", "master",
		NULL};
	int		status;

	if (run(add, s->repo, NULL, 60, &status) != 0 || status != 0)
		return (-1);
	if (run(ci, s->repo, NULL, 60, &status) != 0 || status != 0)
		return (-1);
	if (run(pu, s->repo, NULL, 60, &status) != 0 || status != 0)
		return (-1);
	return (0);
}

static void	report_ko(const t_exercise *ex, const t_report *rep)
{
	printf("  " C_RED "%-4s" C_RESET " %-20s " C_RED "%s" C_RESET "\n", "KO",
		ex->name, verdict_str(rep->verdict));
	if (rep->detail[0])
		printf(C_GREY "         %.400s" C_RESET "\n", rep->detail);
}

/* Sit one assignment: push the reference answer, expect a perfect score. */
static void	sit(t_session *s, const t_exercise *ex, t_tally *t)
{
	t_question	q;
	t_report	rep;
	char		tdir[MAX_PATH];
	char		why[512];

	memset(&q, 0, sizeof(q));
	q.ex = ex;
	q.attempts = 1;
	tdir_of(ex, tdir, sizeof(tdir));
	if (!path_exists(tdir))
	{
		printf("  " C_GREY "%-4s %-20s no tests yet, so no exam draws it"
			C_RESET "\n", "--", ex->name);
		return ;
	}
	repo_reset(s);
	why[0] = '\0';
	if (stage_answer(s, ex, why, sizeof(why)) != 0)
	{
		printf("  " C_RED "%-4s" C_RESET " %-20s " C_RED "%s" C_RESET "\n",
			"KO", ex->name, why);
		t->ko++;
		return ;
	}
	if (push(s, "reference answer") != 0)
	{
		printf("  " C_RED "%-4s" C_RESET " %-20s " C_RED "cannot push to the"
			" exam server" C_RESET "\n", "KO", ex->name);
		t->ko++;
		return ;
	}
	moulinette_run(s, &q, &rep);
	if (rep.verdict == V_OK)
	{
		printf("  " C_GREEN "%-4s" C_RESET " %-20s " C_GREY
			"level %d, %d/%d cases" C_RESET "\n", "ok", ex->name, ex->level,
			rep.passed, rep.total);
		t->ok++;
		return ;
	}
	report_ko(ex, &rep);
	t->ko++;
}

/*
** Some subjects ship an answer next to them in the archive. A student reads
** those as the worked solution, so they are held to the same bar as the
** reference: push it, grade it, and say so when the archive is wrong.
*/
static void	sit_archived(t_session *s, const t_exercise *ex, t_tally *t)
{
	t_question	q;
	t_report	rep;
	char		src[MAX_PATH * 2];
	char		dst[MAX_PATH * 2];
	char		name[MAX_PATH];

	memset(&q, 0, sizeof(q));
	q.ex = ex;
	q.attempts = 1;
	answer_name(ex, name, sizeof(name));
	snprintf(src, sizeof(src), "%s/level%d/%s/%s", es_root(), ex->level,
		ex->name, name);
	if (!path_exists(src))
		return ;
	repo_reset(s);
	snprintf(dst, sizeof(dst), "%s/%s", s->repo, name);
	if (copy_file(src, dst) != 0)
		return ;
	pool_place_given(ex, s->repo);
	if (push(s, "archived answer") != 0)
		return ;
	moulinette_run(s, &q, &rep);
	if (rep.verdict == V_OK)
	{
		printf("  " C_GREEN "%-4s" C_RESET " %-20s " C_GREY
			"level %d, %d/%d cases" C_RESET "\n", "ok", ex->name, ex->level,
			rep.passed, rep.total);
		t->ok++;
		return ;
	}
	report_ko(ex, &rep);
	t->ko++;
}

/* The subject's own "Allowed functions:" line, verbatim. */
static int	allowed_line(const t_exercise *ex, char *out, size_t n)
{
	char	path[MAX_PATH];
	char	*txt;
	char	*p;
	char	*end;

	snprintf(path, sizeof(path), "%s/level%d/%s/subject.en.txt", es_root(),
		ex->level, ex->name);
	txt = read_file(path);
	if (!txt)
		return (-1);
	p = strstr(txt, "Allowed functions");
	if (p)
		p = strchr(p, ':');
	if (!p++)
	{
		free(txt);
		return (-1);
	}
	while (*p == ' ' || *p == '\t')
		p++;
	end = strchr(p, '\n');
	if (end)
		*end = '\0';
	snprintf(out, n, "%s", p);
	free(txt);
	return (0);
}

/*
** Hold the pool to what the subject says. `syms` may name more than the
** subject does -- printf becomes puts or putchar in the compiler's hands, and
** those are the same permission -- but never less: a function the subject
** grants and the linker refuses fails correct work.
*/
static int	subject_allows(const t_exercise *ex, char *why, size_t n)
{
	char	line[256];
	char	*name;
	char	*save;
	int		i;

	if (allowed_line(ex, line, sizeof(line)) != 0)
	{
		snprintf(why, n, "no subject to check the allow-list against");
		return (-1);
	}
	if (strcmp(line, ex->allowed) != 0)
	{
		snprintf(why, n, "subject says \"%s\", the pool says \"%s\"", line,
			ex->allowed);
		return (-1);
	}
	name = strtok_r(line, ", ", &save);
	while (name)
	{
		if (strcmp(name, "None") && strcmp(name, "-"))
		{
			i = 0;
			while (ex->syms[i] && strcmp(ex->syms[i], name))
				i++;
			if (!ex->syms[i])
			{
				snprintf(why, n, "subject allows %s, the linker does not",
					name);
				return (-1);
			}
		}
		name = strtok_r(NULL, ", ", &save);
	}
	return (0);
}

static const t_exercise	*find(const char *name)
{
	const t_exercise	*pool;
	size_t				n;
	size_t				i;

	pool = pool_all(&n);
	i = 0;
	while (i < n)
	{
		if (!strcmp(pool[i].name, name))
			return (&pool[i]);
		i++;
	}
	return (NULL);
}

/*
** The moulinette grades what a program prints, never how it was written. The
** reference walks the alphabet two letters at a time; this one hands the
** whole string to a single write. Same bytes out, so it passes -- an exam
** that only accepted answers shaped like its own reference would be marking
** style, not correctness.
*/
static void	different_but_right(t_session *s, t_tally *t)
{
	const t_exercise	*ex;
	t_question			q;
	t_report			rep;
	char				dst[MAX_PATH * 2];

	ex = find("maff_alpha");
	if (!ex)
		return ;
	memset(&q, 0, sizeof(q));
	q.ex = ex;
	q.attempts = 1;
	repo_reset(s);
	snprintf(dst, sizeof(dst), "%s/maff_alpha.c", s->repo);
	write_file(dst, "#include <unistd.h>\n\nint\tmain(void)\n{\n"
		"\twrite(1, \"aBcDeFgHiJkLmNoPqRsTuVwXyZ\\n\", 27);\n"
		"\treturn (0);\n}\n");
	if (push(s, "a different maff_alpha") != 0)
		return ;
	moulinette_run(s, &q, &rep);
	if (rep.verdict == V_OK)
	{
		printf("  " C_GREEN "%-4s" C_RESET " %-20s " C_GREY "%d/%d cases"
			C_RESET "\n", "ok", "another way in", rep.passed, rep.total);
		t->ok++;
		return ;
	}
	printf("  " C_RED "%-4s" C_RESET " %-20s " C_RED "%s" C_RESET "\n", "KO",
		"another way in", verdict_str(rep.verdict));
	printf(C_GREY "         same output as the reference, so this had to"
		" pass\n         %.300s" C_RESET "\n", rep.detail);
	t->ko++;
}

/*
** The subject's "Allowed functions" line is what the student reads; the pool's
** symbol list is what the linker is held to. They are written out separately,
** so they can drift apart -- and when they do, the exam fails correct work
** for calling something its own subject allows.
*/
static void	allow_lists_agree(t_tally *t)
{
	const t_exercise	*pool;
	size_t				n;
	size_t				i;

	pool = pool_all(&n);
	i = 0;
	while (i < n)
	{
		char	said[256];
		int		bad;

		bad = subject_allows(&pool[i], said, sizeof(said));
		if (bad == 0)
			t->ok++;
		else
		{
			printf("  " C_RED "%-4s" C_RESET " %-20s " C_RED "%s" C_RESET "\n",
				"KO", pool[i].name, said);
			t->ko++;
		}
		i++;
	}
	printf("  " C_GREY "%zu subjects checked against the pool's allow-lists"
		C_RESET "\n", n);
}

/* A pushed branch is a valid submission even when the student calls it main. */
static void	alternate_branch(t_session *s, t_tally *t)
{
	const t_exercise	*ex;
	t_question			q;
	t_report			rep;
	char				why[512];
	int				status;
	char				*branch[] = {"git", "checkout", "--quiet", "-B", "main",
		NULL};
	char				*add[] = {"git", "add", "-A", NULL};
	char				*commit[] = {"git", "commit", "--quiet", "-m",
		"answer on main", NULL};
	char				*push_main[] = {"git", "push", "--quiet", "--force",
		"origin", "main", NULL};

	ex = find("aff_a");
	if (!ex)
		return ;
	memset(&q, 0, sizeof(q));
	q.ex = ex;
	q.attempts = 1;
	if (repo_reset(s) != 0 || run(branch, s->repo, NULL, 60, &status) != 0
		|| status != 0)
		return ;
	why[0] = '\0';
	if (stage_answer(s, ex, why, sizeof(why)) != 0
		|| run(add, s->repo, NULL, 60, &status) != 0 || status != 0
		|| run(commit, s->repo, NULL, 60, &status) != 0 || status != 0
		|| run(push_main, s->repo, NULL, 60, &status) != 0 || status != 0)
		return ;
	moulinette_run(s, &q, &rep);
	if (rep.verdict == V_OK)
	{
		printf("  " C_GREEN "%-4s" C_RESET " %-20s " C_GREY "%d/%d cases"
			C_RESET "\n", "ok", "push from main", rep.passed, rep.total);
		t->ok++;
		return ;
	}
	printf("  " C_RED "%-4s" C_RESET " %-20s " C_RED "%s" C_RESET "\n",
		"KO", "push from main", verdict_str(rep.verdict));
	t->ko++;
}

/* ------------------------------------------------------------------ */
/* the other side: work that must fail, and fail loudly                */
/* ------------------------------------------------------------------ */

static void	scenario(t_tally *t, const char *what, t_verdict want,
		t_verdict got, const char *trace)
{
	char	*body;
	int		complete;

	body = trace && *trace ? read_file(trace) : NULL;
	complete = (body && strstr(body, "error      : ")
			&& strstr(body, verdict_str(got)) && strstr(body, "robot tried:"));
	if (got == want && complete)
	{
		printf("  " C_GREEN "%-4s" C_RESET " %-20s " C_GREY "%s" C_RESET "\n",
			"ok", what, verdict_str(got));
		t->ok++;
		free(body);
		return ;
	}
	printf("  " C_RED "%-4s" C_RESET " %-20s " C_RED "%s" C_RESET "\n", "KO",
		what, got != want ? verdict_str(got) : "incomplete trace");
	if (got == want)
		printf(C_GREY "         expected error and robot-tried trace sections"
			C_RESET "\n");
	else
		printf(C_GREY "         expected %s" C_RESET "\n", verdict_str(want));
	free(body);
	t->ko++;
}

static void	wrong_answer(t_session *s, const t_exercise *ex, t_tally *t)
{
	t_question	q;
	t_report	rep;
	char		dst[MAX_PATH * 2];
	char		name[MAX_PATH];

	memset(&q, 0, sizeof(q));
	q.ex = ex;
	q.attempts = 2;
	repo_reset(s);
	answer_name(ex, name, sizeof(name));
	snprintf(dst, sizeof(dst), "%s/%s", s->repo, name);
	write_file(dst, "#include <unistd.h>\n\nint\tmain(void)\n{\n"
		"\twrite(1, \"nope\\n\", 5);\n\treturn (0);\n}\n");
	if (push(s, "wrong answer") != 0)
		return ;
	moulinette_run(s, &q, &rep);
	scenario(t, "wrong output", V_WRONG, rep.verdict, rep.trace);
}

static void	missing_file(t_session *s, const t_exercise *ex, t_tally *t)
{
	t_question	q;
	t_report	rep;
	char		dst[MAX_PATH * 2];

	memset(&q, 0, sizeof(q));
	q.ex = ex;
	q.attempts = 3;
	repo_reset(s);
	snprintf(dst, sizeof(dst), "%s/wrong_name.c", s->repo);
	write_file(dst, "int\tmain(void)\n{\n\treturn (0);\n}\n");
	if (push(s, "misnamed answer") != 0)
		return ;
	moulinette_run(s, &q, &rep);
	scenario(t, "misnamed file", V_MISSING_FILE, rep.verdict, rep.trace);
}

static void	source_guard(t_session *s, t_tally *t)
{
	char	path[MAX_PATH * 2];
	char	*content;
	int		restored;

	repo_reset(s);
	snprintf(path, sizeof(path), "%s/keep_me.c", s->repo);
	write_file(path, "int\tkeep_me(void)\n{\n\treturn (42);\n}\n");
	if (repo_backup_sources(s) != 0)
		return ;
	write_file(path, "");
	restored = repo_restore_sources(s);
	content = read_file(path);
	if (restored == 1 && content && strstr(content, "return (42)"))
	{
		printf("  " C_GREEN "%-4s" C_RESET " %-20s " C_GREY
			"source restored" C_RESET "\n", "ok", "source safety guard");
		t->ok++;
	}
	else
	{
		printf("  " C_RED "%-4s" C_RESET " %-20s " C_RED
			"source was not restored" C_RESET "\n", "KO",
			"source safety guard");
		t->ko++;
	}
	free(content);
}

/* A correct answer one directory down must be compiled and graded normally. */
static void	nested_file(t_session *s, const t_exercise *ex,
		const char *label, t_tally *t)
{
	t_question	q;
	t_report	rep;
	char		dir[MAX_PATH * 2];
	char		root[MAX_PATH * 2];
	char		dst[MAX_PATH * 3];
	char		name[MAX_PATH];
	char		why[512];

	memset(&q, 0, sizeof(q));
	q.ex = ex;
	q.attempts = 6;
	repo_reset(s);
	answer_name(ex, name, sizeof(name));
	snprintf(dir, sizeof(dir), "%s/%s", s->repo, ex->name);
	mkdirs(dir);
	why[0] = '\0';
	if (stage_answer(s, ex, why, sizeof(why)) != 0)
		return ;
	snprintf(root, sizeof(root), "%s/%s", s->repo, name);
	snprintf(dst, sizeof(dst), "%s/%s", dir, name);
	if (rename(root, dst) != 0)
		return ;
	if (push(s, "answer in a folder") != 0)
		return ;
	moulinette_run(s, &q, &rep);
	if (rep.verdict == V_OK)
	{
		printf("  " C_GREEN "%-4s" C_RESET " %-20s " C_GREY "%d/%d cases"
			C_RESET "\n", "ok", label, rep.passed, rep.total);
		t->ok++;
		return ;
	}
	printf("  " C_RED "%-4s" C_RESET " %-20s " C_RED "%s" C_RESET "\n",
		"KO", label, verdict_str(rep.verdict));
	t->ko++;
}

static void	forbidden(t_session *s, const t_exercise *ex, t_tally *t)
{
	t_question	q;
	t_report	rep;
	char		dst[MAX_PATH * 2];
	char		name[MAX_PATH];

	memset(&q, 0, sizeof(q));
	q.ex = ex;
	q.attempts = 4;
	repo_reset(s);
	answer_name(ex, name, sizeof(name));
	snprintf(dst, sizeof(dst), "%s/%s", s->repo, name);
	write_file(dst, "#include <stdio.h>\n\nint\tmain(void)\n{\n"
		"\tprintf(\"a\\n\");\n\treturn (0);\n}\n");
	if (push(s, "forbidden answer") != 0)
		return ;
	moulinette_run(s, &q, &rep);
	scenario(t, "forbidden library", V_FORBIDDEN, rep.verdict, rep.trace);
}

static void	broken_code(t_session *s, const t_exercise *ex, t_tally *t)
{
	t_question	q;
	t_report	rep;
	char		dst[MAX_PATH * 2];
	char		name[MAX_PATH];

	memset(&q, 0, sizeof(q));
	q.ex = ex;
	q.attempts = 5;
	repo_reset(s);
	answer_name(ex, name, sizeof(name));
	snprintf(dst, sizeof(dst), "%s/%s", s->repo, name);
	write_file(dst, "int\tmain(void)\n{\n\treturn (nope);\n}\n");
	if (push(s, "broken answer") != 0)
		return ;
	moulinette_run(s, &q, &rep);
	scenario(t, "does not compile", V_COMPILE, rep.verdict, rep.trace);
}

static void	nothing_pushed(t_session *s, const t_exercise *ex, t_tally *t)
{
	t_question	q;
	t_report	rep;
	t_session	fresh;

	memset(&q, 0, sizeof(q));
	q.ex = ex;
	q.attempts = 1;
	fresh = *s;
	snprintf(fresh.remote, sizeof(fresh.remote), "%s/empty.git", s->server);
	{
		char	*init[] = {"git", "init", "--bare", "--quiet", fresh.remote,
			NULL};
		int		status;

		if (run(init, NULL, NULL, 60, &status) != 0)
			return ;
	}
	moulinette_run(&fresh, &q, &rep);
	scenario(t, "nothing pushed", V_NO_COMMIT, rep.verdict, rep.trace);
}

/*
** A student who pushes nothing, the wrong name, uncompilable code, a
** forbidden library or a plainly wrong program must be told which of those it
** was -- and must find the attempt written down in exam/traces afterwards.
*/
static void	scenarios(t_session *s, t_tally *t)
{
	const t_exercise	*ex;
	const t_exercise	*nested_named;
	const t_exercise	*wild;
	const t_exercise	*with_header;

	ex = find("aff_a");
	if (!ex)
		return ;
	wild = find("brackets");
	nested_named = find("aff_z");
	with_header = find("ft_list_size");
	printf("\n  " C_BOLD "the rule is the output, not the code" C_RESET
		C_GREY "  (write it another way, it still passes)" C_RESET "\n\n");
	different_but_right(s, t);
	allow_lists_agree(t);
	printf("\n  " C_BOLD "failing on purpose" C_RESET C_GREY
		"  (aff_a, verdict and trace)" C_RESET "\n\n");
	alternate_branch(s, t);
	source_guard(s, t);
	if (nested_named)
		nested_file(s, nested_named, "nested aff_z/aff_z.c", t);
	if (wild)
		nested_file(s, wild, "nested wildcard file", t);
	if (with_header)
		nested_file(s, with_header, "nested source + header", t);
	nothing_pushed(s, ex, t);
	missing_file(s, ex, t);
	broken_code(s, ex, t);
	forbidden(s, ex, t);
	wrong_answer(s, ex, t);
}

static void	schedule_stays_on_level(t_tally *t)
{
	const t_exercise	*pool;
	const t_exam		*exams;
	t_history			h;
	t_session			planned;
	size_t				npool;
	size_t				nexams;
	size_t				i;
	size_t				j;
	int					valid;

	memset(&h, 0, sizeof(h));
	pool = pool_all(&npool);
	i = 0;
	while (i < npool && h.n < MAX_HISTORY)
	{
		if (pool[i].level == 0)
			snprintf(h.name[h.n++], sizeof(h.name[0]), "%s", pool[i].name);
		i++;
	}
	exams = exam_all(&nexams);
	i = 0;
	while (i < nexams)
	{
		memset(&planned, 0, sizeof(planned));
		planned.exam = &exams[i];
		valid = (session_plan_questions(&planned, &h) == 0);
		j = 0;
		while (valid && j < planned.nq)
		{
			if (planned.q[j].ex->level != exams[i].slots[j].level)
				valid = 0;
			j++;
		}
		if (valid && planned.nq && planned.q[0].ex->level == 0)
		{
			printf("  " C_GREEN "ok  " C_RESET " %-20s" C_GREY
				"starts at level 0" C_RESET "\n", exams[i].id);
			t->ok++;
		}
		else
		{
			printf("  " C_RED "KO  " C_RESET " %-20s"
				"left its configured level\n", exams[i].id);
			t->ko++;
		}
		i++;
	}
}

/* ------------------------------------------------------------------ */

int	selftest(int only_level)
{
	t_session			s;
	const t_exercise	*pool;
	t_tally				t;
	size_t				n;
	size_t				i;

	pool = pool_all(&n);
	memset(&s, 0, sizeof(s));
	s.exam = exam_by_index(0);
	if (repo_plan(&s) != 0 || repo_setup(&s) != 0)
	{
		fprintf(stderr, "selftest: cannot create scratch repository\n");
		return (1);
	}
	t.ok = 0;
	t.ko = 0;
	if (only_level < 0)
	{
		printf("\n  " C_BOLD "stable exam schedules" C_RESET C_GREY
			"  (level 0 history fully exhausted)" C_RESET "\n\n");
		schedule_stays_on_level(&t);
	}
	printf("\n  " C_BOLD "sitting every assignment" C_RESET C_GREY
		"  (reference answer, pushed and graded)" C_RESET "\n\n");
	i = 0;
	while (i < n)
	{
		if (only_level < 0 || pool[i].level == only_level)
			sit(&s, &pool[i], &t);
		i++;
	}
	printf("\n  " C_BOLD "the answers the archive ships" C_RESET C_GREY
		"  (where there is one)" C_RESET "\n\n");
	i = 0;
	while (i < n)
	{
		if (only_level < 0 || pool[i].level == only_level)
			sit_archived(&s, &pool[i], &t);
		i++;
	}
	if (only_level < 0)
		scenarios(&s, &t);
	printf("\n  " C_BOLD "%d ok" C_RESET ", %s%d ko" C_RESET "\n\n", t.ok,
		t.ko ? C_RED C_BOLD : C_BOLD, t.ko);
	rm_rf(s.workspace);
	return (t.ko ? 1 : 0);
}
