/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   moulinette.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:25:50 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 02:30:24 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "examshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <ctype.h>
#include <time.h>
#include <sys/wait.h>

#define RUN_TIMEOUT 10
#define MAX_SRC 32
#define MAX_ARGS 64
#define TREE_ENTRIES 24

/*
** The moulinette. It never reads the student's working copy: everything is
** graded from the tarball exported out of the pushed branch.
**
** A submission is accepted only when it (1) contains the expected files,
** (2) references no forbidden libc symbol, (3) compiles clean with
** -Wall -Wextra -Werror, and (4) produces byte-for-byte the same output as
** the reference implementation on every test case. Anything else is a 0,
** all-or-nothing, like the real thing.
*/

const char	*verdict_str(t_verdict v)
{
	if (v == V_OK)
		return ("OK");
	if (v == V_NO_COMMIT)
		return ("Nothing pushed");
	if (v == V_MISSING_FILE)
		return ("Missing file");
	if (v == V_FORBIDDEN)
		return ("Forbidden function");
	if (v == V_COMPILE)
		return ("Does not compile");
	if (v == V_WRONG)
		return ("Wrong output");
	if (v == V_TIMEOUT)
		return ("Timeout");
	if (v == V_CRASH)
		return ("Crash");
	return ("Internal error");
}

/* ------------------------------------------------------------------ */
/* shell-ish tokenizer for the argument case files                     */
/* ------------------------------------------------------------------ */

static int	tokenize(const char *line, char **out, int max)
{
	char	buf[4096];
	int		n;
	size_t	b;
	int		quote;

	n = 0;
	while (*line && n < max - 1)
	{
		while (*line == ' ' || *line == '\t')
			line++;
		if (!*line)
			break ;
		b = 0;
		quote = 0;
		while (*line && (quote || (*line != ' ' && *line != '\t'))
			&& b < sizeof(buf) - 1)
		{
			if (!quote && (*line == '\'' || *line == '"'))
				quote = *line++;
			else if (quote && *line == quote)
			{
				quote = 0;
				line++;
			}
			else if (*line == '\\' && line[1])
			{
				line++;
				if (*line == 'n')
					buf[b++] = '\n';
				else if (*line == 't')
					buf[b++] = '\t';
				else
					buf[b++] = *line;
				line++;
			}
			else
				buf[b++] = *line++;
		}
		buf[b] = '\0';
		out[n++] = es_strdup(buf);
	}
	out[n] = NULL;
	return (n);
}

static void	free_tokens(char **t)
{
	int	i;

	i = 0;
	while (t[i])
		free(t[i++]);
}

/* ------------------------------------------------------------------ */
/* source discovery                                                    */
/* ------------------------------------------------------------------ */

typedef struct s_match
{
	char	path[MAX_PATH * 2];
	int		count;
} t_match;

static void	find_named(const char *dir, const char *name, t_match *match)
{
	DIR				*d;
	struct dirent	*e;
	char			child[MAX_PATH * 2];

	d = opendir(dir);
	if (!d)
		return ;
	while ((e = readdir(d)))
	{
		if (e->d_name[0] == '.')
			continue ;
		snprintf(child, sizeof(child), "%s/%s", dir, e->d_name);
		if (is_dir(child))
			find_named(child, name, match);
		else if (!strcmp(e->d_name, name))
		{
			if (match->count == 0)
				snprintf(match->path, sizeof(match->path), "%s", child);
			match->count++;
		}
	}
	closedir(d);
}

static void	collect_c_tree(const char *dir, char **src, int *n)
{
	DIR				*d;
	struct dirent	*e;
	char			child[MAX_PATH * 2];

	d = opendir(dir);
	if (!d)
		return ;
	while ((e = readdir(d)) && *n < MAX_SRC - 1)
	{
		size_t	len;

		if (e->d_name[0] == '.')
			continue ;
		snprintf(child, sizeof(child), "%s/%s", dir, e->d_name);
		if (is_dir(child))
			collect_c_tree(child, src, n);
		else
		{
			len = strlen(e->d_name);
			if (len > 2 && !strcmp(e->d_name + len - 2, ".c"))
				src[(*n)++] = es_strdup(child);
		}
	}
	closedir(d);
}

static int	collect_sources(const t_exercise *ex, const char *dir, char **src,
		char *missing, size_t missing_n)
{
	int				n;
	int				i;
	char			path[MAX_PATH];

	n = 0;
	if (!strcmp(ex->files[0], "*.c"))
	{
		collect_c_tree(dir, src, &n);
		if (n == 0)
		{
			snprintf(missing, missing_n, "*.c");
			return (-1);
		}
		src[n] = NULL;
		return (n);
	}
	i = 0;
	while (ex->files[i])
	{
		t_match	match;

		snprintf(path, sizeof(path), "%s/%s", dir, ex->files[i]);
		if (!path_exists(path) || is_dir(path))
		{
			memset(&match, 0, sizeof(match));
			find_named(dir, ex->files[i], &match);
			if (match.count == 0)
			{
				snprintf(missing, missing_n, "%s", ex->files[i]);
				return (-1);
			}
			if (match.count > 1)
			{
				snprintf(missing, missing_n, "%s", ex->files[i]);
				return (-2);
			}
			snprintf(path, sizeof(path), "%s", match.path);
		}
		if (strlen(ex->files[i]) > 2
			&& !strcmp(ex->files[i] + strlen(ex->files[i]) - 2, ".c"))
			src[n++] = es_strdup(path);
		i++;
	}
	src[n] = NULL;
	return (n);
}

/* Make a required student header visible beside every nested source file. */
static int	place_expected_headers(const t_exercise *ex, const char *rendu,
		char **src, int nsrc)
{
	char	from[MAX_PATH * 2];
	char	dir[MAX_PATH * 2];
	char	to[MAX_PATH * 2];
	char	*slash;
	t_match	match;
	int		i;
	int		j;

	j = 0;
	while (ex->files[j])
	{
		size_t	len = strlen(ex->files[j]);

		if (len <= 2 || strcmp(ex->files[j] + len - 2, ".h"))
		{
			j++;
			continue ;
		}
		snprintf(from, sizeof(from), "%s/%s", rendu, ex->files[j]);
		if (!path_exists(from) || is_dir(from))
		{
			memset(&match, 0, sizeof(match));
			find_named(rendu, ex->files[j], &match);
			if (match.count != 1)
				return (-1);
			snprintf(from, sizeof(from), "%s", match.path);
		}
		i = 0;
		while (i < nsrc)
		{
			snprintf(dir, sizeof(dir), "%s", src[i]);
			slash = strrchr(dir, '/');
			if (slash)
				*slash = '\0';
			snprintf(to, sizeof(to), "%s/%s", dir, ex->files[j]);
			if (strcmp(from, to) && copy_file(from, to) != 0)
				return (-1);
			i++;
		}
		j++;
	}
	return (0);
}

/* Put grader-provided headers next to nested student sources as well. */
static int	place_given_near_sources(const t_exercise *ex, const char *tdir,
		char **src, int nsrc)
{
	char	from[MAX_PATH * 2];
	char	dir[MAX_PATH * 2];
	char	to[MAX_PATH * 2];
	char	*slash;
	int		i;
	int		j;

	i = 0;
	while (i < nsrc)
	{
		snprintf(dir, sizeof(dir), "%s", src[i]);
		slash = strrchr(dir, '/');
		if (slash)
			*slash = '\0';
		j = 0;
		while (ex->given[j])
		{
			snprintf(from, sizeof(from), "%s/given/%s", tdir, ex->given[j]);
			snprintf(to, sizeof(to), "%s/%s", dir, ex->given[j]);
			if (copy_file(from, to) != 0)
				return (-1);
			j++;
		}
		i++;
	}
	return (0);
}

/*
** List what actually reached the exam server, path by path, directories and
** all. "I pushed it and it says the file is missing" is nearly always a file
** sitting one directory down, or under a name that is off by a character, so
** the answer to it is to show the student the tree the grader was handed.
**
** The listing gets a budget of its own rather than the whole report: whatever
** was pushed, the line that says what to do about it has to survive.
*/
typedef struct s_tree
{
	char	text[640];
	int		left;
}	t_tree;
static void	list_pushed(const char *dir, const char *prefix, t_tree *t,
		size_t *o)
{
	DIR				*d;
	struct dirent	*e;
	char			rel[MAX_PATH];
	char			child[MAX_PATH * 2];

	d = opendir(dir);
	if (!d)
		return ;
	while ((e = readdir(d)) && t->left > 0)
	{
		if (e->d_name[0] == '.')
			continue ;
		snprintf(rel, sizeof(rel), "%s%s", prefix, e->d_name);
		snprintf(child, sizeof(child), "%s/%s", dir, e->d_name);
		t->left--;
		if (!is_dir(child))
			appendf(t->text, sizeof(t->text), o, "  %s\n", rel);
		else
		{
			appendf(t->text, sizeof(t->text), o, "  %s/\n", rel);
			snprintf(rel, sizeof(rel), "%s%s/", prefix, e->d_name);
			list_pushed(child, rel, t, o);
		}
	}
	closedir(d);
}

/* The subject's "Expected files" line, rebuilt from the pool entry. */
static void	expected_files(const t_exercise *ex, char *out, size_t size)
{
	size_t	o;
	int		i;

	if (!strcmp(ex->files[0], "*.c"))
	{
		snprintf(out, size, "*.c, *.h");
		return ;
	}
	o = 0;
	i = 0;
	while (ex->files[i])
	{
		appendf(out, size, &o, "%s%s", i ? ", " : "", ex->files[i]);
		i++;
	}
}

static void	missing_detail(t_report *rep, const t_exercise *ex,
		const char *missing, const char *rendu)
{
	char	want[256];
	t_tree	tree;
	size_t	o;

	expected_files(ex, want, sizeof(want));
	tree.text[0] = '\0';
	tree.left = TREE_ENTRIES;
	o = 0;
	list_pushed(rendu, "", &tree, &o);
	if (!tree.text[0])
		appendf(tree.text, sizeof(tree.text), &o, "  (nothing)\n");
	else if (tree.left == 0)
		appendf(tree.text, sizeof(tree.text), &o, "  ...\n");
	o = 0;
	appendf(rep->detail, sizeof(rep->detail), &o,
		"Expected file not found in what you pushed: %s\n"
		"Expected files: %s\n\nYour push holds:\n%s\n"
		"Files may be at the repository root or in a subdirectory, but their"
		" names must be spelled exactly as above.", missing, want, tree.text);
}

static void	ambiguous_detail(t_report *rep, const t_exercise *ex,
		const char *name, const char *rendu)
{
	char	want[256];
	t_tree	tree;
	size_t	o;

	expected_files(ex, want, sizeof(want));
	tree.text[0] = '\0';
	tree.left = TREE_ENTRIES;
	o = 0;
	list_pushed(rendu, "", &tree, &o);
	o = 0;
	appendf(rep->detail, sizeof(rep->detail), &o,
		"More than one pushed file is named %s.\nExpected files: %s\n\n"
		"Your push holds:\n%s\nKeep only one matching file, or put the"
		" intended file at the repository root.", name, want, tree.text);
}

/* ------------------------------------------------------------------ */
/* forbidden function detection                                        */
/* ------------------------------------------------------------------ */

/*
** Symbols the toolchain itself can emit. The compile flags below stop the
** active compiler from inventing memcpy/memset calls out of plain loops, so
** these are the only ones that are not the student's doing.
*/
static const char	*g_toolchain[] = {
	"_GLOBAL_OFFSET_TABLE_", "__stack_chk_fail", "__stack_chk_guard",
	"__libc_start_main", "__gmon_start__", "_ITM_deregisterTMCloneTable",
	"_ITM_registerTMCloneTable", "__cxa_finalize", "__errno_location",
	"__assert_fail", NULL
};

static int	sym_allowed(const t_exercise *ex, const char *sym)
{
	int	i;

	i = 0;
	while (g_toolchain[i])
		if (!strcmp(g_toolchain[i++], sym))
			return (1);
	i = 0;
	while (ex->syms[i])
	{
		if (!strcmp(ex->syms[i], sym))
			return (1);
		i++;
	}
	return (0);
}

/*
** Which libc header hands out which gated function. A system header is only
** acceptable when the subject allows something it declares: if all you may
** call is write, then <unistd.h> is legitimate and <string.h> is not, even
** before you call anything out of it.
**
** Headers that declare no functions at all (types and limits only) are
** always fine, and so is any "quoted.h" of the student's own making.
*/
static const char	*g_type_only[] = {
	"stddef.h", "stdint.h", "stdbool.h", "limits.h", "stdarg.h", "errno.h",
	NULL
};

static const struct s_hdr
{
	const char	*header;
	const char	*fn;
}	g_headers[] = {
	{"unistd.h", "write"},
	{"stdio.h", "printf"},
	{"stdlib.h", "malloc"},
	{"stdlib.h", "free"},
	{"stdlib.h", "exit"},
	{"stdlib.h", "atoi"},
	{NULL, NULL}
};

static int	header_allowed(const t_exercise *ex, const char *hdr)
{
	int	i;

	i = 0;
	while (g_type_only[i])
		if (!strcmp(g_type_only[i++], hdr))
			return (1);
	i = 0;
	while (g_headers[i].header)
	{
		if (!strcmp(g_headers[i].header, hdr)
			&& sym_allowed(ex, g_headers[i].fn))
			return (1);
		i++;
	}
	return (0);
}

static int	check_includes(const t_exercise *ex, char **src, int nsrc,
		t_report *rep)
{
	int		i;

	i = 0;
	while (i < nsrc)
	{
		char	*txt;
		char	*p;

		txt = read_file(src[i]);
		if (!txt)
			return (0);
		p = txt;
		while ((p = strstr(p, "#include")) != NULL)
		{
			char	hdr[128];
			char	*open;
			char	*close;
			size_t	len;

			p += 8;
			while (*p == ' ' || *p == '\t')
				p++;
			if (*p != '<')
				continue ;
			open = p + 1;
			close = strchr(open, '>');
			if (!close)
				continue ;
			len = (size_t)(close - open);
			if (len >= sizeof(hdr))
				len = sizeof(hdr) - 1;
			memcpy(hdr, open, len);
			hdr[len] = '\0';
			if (!header_allowed(ex, hdr))
			{
				snprintf(rep->detail, sizeof(rep->detail),
					"Forbidden library: <%s>\n"
					"Allowed functions for this assignment: %s\n"
					"Only the headers that declare them may be included.",
					hdr, *ex->allowed ? ex->allowed : "none");
				free(txt);
				return (-1);
			}
			p = close;
		}
		free(txt);
		i++;
	}
	return (0);
}

/* Strip glibc decorations: printf@GLIBC_2.2.5 / __isoc99_scanf -> scanf */
static void	normalize_sym(char *s)
{
	char	*at;

	at = strchr(s, '@');
	if (at)
		*at = '\0';
	if (!strncmp(s, "__isoc99_", 9))
		memmove(s, s + 9, strlen(s + 9) + 1);
}

/*
** A line of `nm -u` is "<spaces>U <symbol>", and nm also prints a
** "file.o:" banner per object when handed several. The symbol is simply the
** last whitespace-separated token; banners and the lone type letter are not.
*/
static char	*last_token(char *line)
{
	char	*tok;
	size_t	n;

	n = strlen(line);
	while (n && (line[n - 1] == ' ' || line[n - 1] == '\t'
			|| line[n - 1] == '\r'))
		line[--n] = '\0';
	if (!n || line[n - 1] == ':')
		return (NULL);
	tok = strrchr(line, ' ');
	if (tok)
		tok++;
	else
		tok = line;
	if (!strcmp(tok, "U") || !strcmp(tok, "w") || !*tok)
		return (NULL);
	return (tok);
}

static int	check_symbols(const t_exercise *ex, char **objs, int nobj,
		t_report *rep)
{
	char	*out;
	char	*line;
	char	*save;
	char	sym[256];
	char	*argv[MAX_SRC + 3];
	int		i;
	int		status;

	argv[0] = "nm";
	argv[1] = "-u";
	i = 0;
	while (i < nobj)
	{
		argv[2 + i] = objs[i];
		i++;
	}
	argv[2 + nobj] = NULL;
	out = NULL;
	if (run_capture(argv, NULL, &out, 30, &status) != 0 || !out)
	{
		free(out);
		return (0);
	}
	line = strtok_r(out, "\n", &save);
	while (line)
	{
		char	*tok;

		tok = last_token(line);
		if (tok)
		{
			snprintf(sym, sizeof(sym), "%s", tok);
			normalize_sym(sym);
			if (*sym && !sym_allowed(ex, sym))
			{
				snprintf(rep->detail, sizeof(rep->detail),
					"Forbidden function detected: %s\n"
					"Allowed functions for this assignment: %s", sym,
					*ex->allowed ? ex->allowed : "none");
				free(out);
				return (-1);
			}
		}
		line = strtok_r(NULL, "\n", &save);
	}
	free(out);
	return (0);
}

/* ------------------------------------------------------------------ */
/* compilation                                                         */
/* ------------------------------------------------------------------ */

/*
** `incdir` is where the headers the exam provides have been laid down. Both
** sides of the comparison need it on the include path: the student's own
** copy sits next to their sources, but the reference is compiled straight out
** of the exercise archive, while provided headers live under given/.
*/
static void	cc_flags(char **argv, int *n, const char *incdir)
{
	argv[(*n)++] = "-Wall";
	argv[(*n)++] = "-Wextra";
	argv[(*n)++] = "-Werror";
	argv[(*n)++] = "-fno-builtin";
	argv[(*n)++] = "-fno-stack-protector";
	/* GCC needs this to avoid synthesising libc calls; clang rejects it. */
#if defined(__GNUC__) && !defined(__clang__)
	argv[(*n)++] = "-fno-tree-loop-distribute-patterns";
#endif
	argv[(*n)++] = "-g";
	argv[(*n)++] = "-I";
	argv[(*n)++] = (char *)incdir;
}

static int	compile_objects(char **src, int nsrc, const char *outdir,
		char **objs, char *log)
{
	char	*argv[MAX_SRC + 24];
	char	obj[MAX_PATH];
	int		i;
	int		n;
	int		status;
	int		failed;

	failed = 0;
	i = 0;
	while (i < nsrc)
	{
		snprintf(obj, sizeof(obj), "%s/source_%02d.o", outdir, i);
		n = 0;
		argv[n++] = "cc";
		cc_flags(argv, &n, outdir);
		argv[n++] = "-c";
		argv[n++] = src[i];
		argv[n++] = "-o";
		argv[n++] = obj;
		argv[n] = NULL;
		if (run(argv, NULL, log, 60, &status) != 0 || status != 0)
			failed = 1;
		objs[i] = es_strdup(obj);
		i++;
	}
	objs[nsrc] = NULL;
	return (failed ? -1 : 0);
}

static int	link_binary(char **objs, int nobj, const char *out, char *log)
{
	char	*argv[MAX_SRC + 8];
	int		n;
	int		i;
	int		status;

	n = 0;
	argv[n++] = "cc";
	i = 0;
	while (i < nobj)
		argv[n++] = objs[i++];
	argv[n++] = "-o";
	argv[n++] = (char *)out;
	argv[n] = NULL;
	if (run(argv, NULL, log, 60, &status) != 0 || status != 0)
		return (-1);
	return (0);
}

/* ------------------------------------------------------------------ */
/* running one case                                                    */
/* ------------------------------------------------------------------ */

static int	exit_code(int status)
{
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

static int	run_case(const char *bin, char **args, const char *envline,
		char **out, int *code)
{
	char	*argv[MAX_ARGS + 8];
	int		n;
	int		i;
	int		status;
	int		timed_out;

	n = 0;
	if (envline && *envline)
	{
		argv[n++] = "env";
		argv[n++] = (char *)envline;
	}
	argv[n++] = (char *)bin;
	i = 0;
	while (args && args[i] && n < MAX_ARGS)
		argv[n++] = args[i++];
	argv[n] = NULL;
	status = 0;
	timed_out = run_capture(argv, NULL, out, RUN_TIMEOUT, &status);
	*code = exit_code(status);
	return (timed_out);
}

static void	snippet(char *dst, size_t n, const char *s)
{
	size_t	i;
	size_t	o;

	o = 0;
	i = 0;
	while (s && s[i] && o + 5 < n)
	{
		if (s[i] == '\n')
		{
			dst[o++] = '$';
			dst[o++] = '\\';
			dst[o++] = 'n';
		}
		else if (s[i] == '\t')
		{
			dst[o++] = '\\';
			dst[o++] = 't';
		}
		else
			dst[o++] = s[i];
		i++;
	}
	dst[o] = '\0';
}


/* ------------------------------------------------------------------ */
/* test log                                                            */
/* ------------------------------------------------------------------ */

/*
** Where the two outputs first part company, 1-based. Used to point at the
** offending line of the submission's own output -- the reference's line is
** never shown, so the log says "this broke" and not "here is the answer".
*/
static int	first_diff_line(const char *a, const char *b)
{
	int	line;

	line = 1;
	while (*a && *b && *a == *b)
	{
		if (*a == '\n')
			line++;
		a++;
		b++;
	}
	return (line);
}

static void	nth_line(const char *s, int n, char *out, size_t size)
{
	int		line;
	size_t	i;

	line = 1;
	while (*s && line < n)
		if (*s++ == '\n')
			line++;
	i = 0;
	while (*s && *s != '\n' && i + 1 < size)
		out[i++] = *s++;
	out[i] = '\0';
}

/* The exam is all-or-nothing: any broken case sinks the whole submission. */
static t_verdict	verdict_of(const t_report *rep)
{
	int	i;

	i = 0;
	while (i < rep->ncases)
	{
		if (rep->cases[i].verdict != V_OK)
			return (rep->cases[i].verdict);
		i++;
	}
	return (V_OK);
}

static void	record(t_report *rep, const char *args, t_verdict v,
		const char *sout, const char *rout)
{
	t_case_result	*c;

	if (rep->ncases >= MAX_CASES)
		return ;
	c = &rep->cases[rep->ncases++];
	snprintf(c->args, sizeof(c->args), "%s", args);
	c->verdict = v;
	c->ran = 1;
	c->diff_line = 0;
	c->got[0] = '\0';
	if (v == V_OK)
	{
		rep->passed++;
		return ;
	}
	if (v == V_WRONG && sout && rout)
	{
		char	raw[256];

		c->diff_line = first_diff_line(sout, rout);
		nth_line(sout, c->diff_line, raw, sizeof(raw));
		snippet(c->got, sizeof(c->got), raw);
	}
	else if (sout)
	{
		char	raw[256];

		snprintf(raw, sizeof(raw), "%s", sout);
		snippet(c->got, sizeof(c->got), raw);
	}
}

/*
** The trace kept on disk mirrors the on-screen log, attempt by attempt, so
** the history of a question can be re-read later. Same rule: what happened,
** never what was expected.
*/
static void	trace_head(t_question *q, t_report *rep, char *body, size_t size,
		size_t *o)
{
	char	when[64];
	time_t	now;

	now = time(NULL);
	strftime(when, sizeof(when), "%Y-%m-%d %H:%M:%S", localtime(&now));
	*o = 0;
	appendf(body, size, o,
		"assignment : %s (level %d)\nattempt    : %d\ntime       : %s\n"
		"error      : %s\ncases      : %d/%d passed\n", q->ex->name,
		q->ex->level, q->attempts, when, verdict_str(rep->verdict),
		rep->passed, rep->total);
	if (rep->detail[0])
		appendf(body, size, o, "\n%s\n", rep->detail);
	appendf(body, size, o, "\n");
}

static void	trace_body(t_question *q, t_report *rep, char *body, size_t size)
{
	int		i;
	size_t	o;

	trace_head(q, rep, body, size, &o);
	appendf(body, size, &o, "robot tried:\n");
	if (rep->ncases == 0)
		appendf(body, size, &o, "  (no test case ran)\n");
	i = 0;
	while (i < rep->ncases)
	{
		t_case_result	*c = &rep->cases[i];

		appendf(body, size, &o, "  [%s] ./%s %s\n",
			c->verdict == V_OK ? "ok" : "KO", q->ex->name, c->args);
		if (c->verdict == V_WRONG)
			appendf(body, size, &o, "         your output, line %d: %s\n",
				c->diff_line, c->got);
		else if (c->verdict != V_OK)
			appendf(body, size, &o, "         %s\n", verdict_str(c->verdict));
		i++;
	}
}

/* ------------------------------------------------------------------ */
/* test-case files                                                     */
/* ------------------------------------------------------------------ */

typedef struct s_cases
{
	char	*lines[256];
	int		n;
}	t_cases;

/*
** One case per line, in argv syntax. An empty line means "run it with no
** arguments at all" -- which is a case almost every subject asks about --
** so the split has to keep empty lines, unlike strtok(). Lines opening with
** '#' are comments.
*/
static int	load_cases(const char *tdir, t_cases *c)
{
	char	path[MAX_PATH];
	char	*data;
	char	*p;
	char	*line;

	snprintf(path, sizeof(path), "%s/args.txt", tdir);
	data = read_file(path);
	c->n = 0;
	if (!data)
		return (-1);
	p = data;
	if (*p && p[strlen(p) - 1] == '\n')
		p[strlen(p) - 1] = '\0';
	line = p;
	while (c->n < 256)
	{
		p = strchr(line, '\n');
		if (p)
			*p = '\0';
		if (line[0] != '#')
			c->lines[c->n++] = es_strdup(line);
		if (!p)
			break ;
		line = p + 1;
	}
	free(data);
	return (0);
}

static void	free_cases(t_cases *c)
{
	int	i;

	i = 0;
	while (i < c->n)
		free(c->lines[i++]);
}

/* ------------------------------------------------------------------ */
/* traces                                                              */
/* ------------------------------------------------------------------ */

static void	write_trace(t_session *s, t_question *q, t_report *rep)
{
	char	path[MAX_PATH * 2];
	char	dir[MAX_PATH * 2];
	char	body[8192];

	snprintf(dir, sizeof(dir), "%s/traces", s->docs);
	mkdirs(dir);
	snprintf(path, sizeof(path), "%s/%s_attempt%d.trace", dir, q->ex->name,
		q->attempts);
	trace_body(q, rep, body, sizeof(body));
	write_file(path, body);
	snprintf(rep->trace, sizeof(rep->trace), "%s", path);
}

/* ------------------------------------------------------------------ */
/* the grader                                                          */
/* ------------------------------------------------------------------ */

/*
** Build one side of the comparison. `nsym` is how many of the leading
** objects belong to the student: the harness we append for function
** assignments is ours, so its printf() must never count against them.
*/
static int	build_side(const t_exercise *ex, char **src, int nsrc,
		const char *dir, const char *bin, char *log, t_report *rep,
		int is_student, int nsym)
{
	char	*objs[MAX_SRC + 1];
	int		i;
	int		r;

	pool_place_given(ex, dir);
	r = compile_objects(src, nsrc, dir, objs, log);
	if (r != 0)
	{
		if (is_student)
			rep->verdict = V_COMPILE;
		else
			rep->verdict = V_INTERNAL;
		i = 0;
		while (objs[i])
			free(objs[i++]);
		return (-1);
	}
	if (is_student && nsym > 0 && check_includes(ex, src, nsym, rep) != 0)
	{
		rep->verdict = V_FORBIDDEN;
		i = 0;
		while (objs[i])
			free(objs[i++]);
		return (-1);
	}
	if (is_student && nsym > 0 && check_symbols(ex, objs, nsym, rep) != 0)
	{
		rep->verdict = V_FORBIDDEN;
		i = 0;
		while (objs[i])
			free(objs[i++]);
		return (-1);
	}
	r = link_binary(objs, nsrc, bin, log);
	i = 0;
	while (objs[i])
		free(objs[i++]);
	if (r != 0)
	{
		rep->verdict = is_student ? V_COMPILE : V_INTERNAL;
		return (-1);
	}
	return (0);
}

static void	compile_detail(t_report *rep, const char *log)
{
	char	*txt;

	txt = read_file(log);
	if (!txt)
		return ;
	snprintf(rep->detail, sizeof(rep->detail),
		"cc -Wall -Wextra -Werror refused your files:\n\n%.900s", txt);
	free(txt);
}

static int	grade(t_session *s, t_question *q, t_report *rep)
{
	char		rendu[MAX_PATH * 2];
	char		build[MAX_PATH * 2];
	char		tdir[MAX_PATH * 2];
	char		log[MAX_PATH * 2];
	char		sbin[MAX_PATH * 3];
	char		rbin[MAX_PATH * 3];
	char		*ssrc[MAX_SRC + 1];
	char		*rsrc[MAX_SRC + 1];
	char		missing[256];
	t_cases		cases;
	int			nsrc;
	int			nstudent;
	int			i;

	memset(rep, 0, sizeof(*rep));
	rep->verdict = V_INTERNAL;
	snprintf(rendu, sizeof(rendu), "%s/rendu", s->tmp);
	snprintf(build, sizeof(build), "%s/build", s->tmp);
	snprintf(tdir, sizeof(tdir), "%s/level%d/%s", es_root(),
		q->ex->level, q->ex->name);
	snprintf(log, sizeof(log), "%s/cc.log", s->tmp);
	snprintf(sbin, sizeof(sbin), "%s/student", build);
	snprintf(rbin, sizeof(rbin), "%s/reference", build);
	rm_rf(build);
	mkdirs(build);
	if (repo_collect(s, rendu) != 0)
	{
		rep->verdict = V_NO_COMMIT;
		snprintf(rep->detail, sizeof(rep->detail),
			"Nothing was pushed to your repository yet.\n"
			"Remember: git add, git commit, then git push.");
		return (-1);
	}
	nsrc = collect_sources(q->ex, rendu, ssrc, missing, sizeof(missing));
	if (nsrc == -2)
	{
		rep->verdict = V_MISSING_FILE;
		ambiguous_detail(rep, q->ex, missing, rendu);
		return (-1);
	}
	if (nsrc < 0)
	{
		rep->verdict = V_MISSING_FILE;
		missing_detail(rep, q->ex, missing, rendu);
		return (-1);
	}
	pool_place_given(q->ex, rendu);
	nstudent = nsrc;
	if (place_expected_headers(q->ex, rendu, ssrc, nstudent) != 0
		|| place_given_near_sources(q->ex, tdir, ssrc, nstudent) != 0)
	{
		rep->verdict = V_INTERNAL;
		snprintf(rep->detail, sizeof(rep->detail),
			"Could not place the grader-provided headers.");
		return (-1);
	}
	/* the harness supplies main() for function assignments */
	if (q->ex->type == EX_FUNCTION)
	{
		char	h[MAX_PATH];

		snprintf(h, sizeof(h), "%s/harness.c", tdir);
		ssrc[nsrc++] = es_strdup(h);
		ssrc[nsrc] = NULL;
	}
	if (build_side(q->ex, ssrc, nsrc, build, sbin, log, rep, 1, nstudent) != 0)
	{
		if (rep->verdict == V_COMPILE)
			compile_detail(rep, log);
		i = 0;
		while (i < nsrc)
			free(ssrc[i++]);
		return (-1);
	}
	i = 0;
	while (i < nsrc)
		free(ssrc[i++]);
	/* ---- reference side ---- */
	{
		char	refc[MAX_PATH];
		char	refdir[MAX_PATH];
		int		nref;

		snprintf(refdir, sizeof(refdir), "%s/ref", build);
		mkdirs(refdir);
		snprintf(refc, sizeof(refc), "%s/ref.c", tdir);
		nref = 0;
		rsrc[nref++] = es_strdup(refc);
		if (q->ex->type == EX_FUNCTION)
		{
			char	h[MAX_PATH];

			snprintf(h, sizeof(h), "%s/harness.c", tdir);
			rsrc[nref++] = es_strdup(h);
		}
		rsrc[nref] = NULL;
		if (build_side(q->ex, rsrc, nref, refdir, rbin, log, rep, 0, 0) != 0)
		{
			snprintf(rep->detail, sizeof(rep->detail),
				"The reference build failed. This is a bug in examshell,"
				" not in your code.");
			i = 0;
			while (i < nref)
				free(rsrc[i++]);
			return (-1);
		}
		i = 0;
		while (i < nref)
			free(rsrc[i++]);
	}
	/* ---- run every case ---- */
	if (q->ex->type == EX_FUNCTION)
	{
		cases.n = 1;
		cases.lines[0] = es_strdup("");
	}
	else if (load_cases(tdir, &cases) != 0)
	{
		snprintf(rep->detail, sizeof(rep->detail),
			"Missing test cases for %s. This is a bug in examshell.",
			q->ex->name);
		return (-1);
	}
	rep->total = cases.n;
	i = 0;
	while (i < cases.n)
	{
		char	*args[MAX_ARGS];
		char	*sout;
		char	*rout;
		int		scode;
		int		rcode;
		int		stimeout;

		tokenize(cases.lines[i], args, MAX_ARGS);
		sout = NULL;
		rout = NULL;
		stimeout = run_case(sbin, args, NULL, &sout, &scode);
		run_case(rbin, args, NULL, &rout, &rcode);
		if (stimeout)
			record(rep, cases.lines[i], V_TIMEOUT, NULL, NULL);
		else if (scode >= 128 && rcode < 128)
			record(rep, cases.lines[i], V_CRASH, NULL, NULL);
		else if (!sout || !rout || strcmp(sout, rout) != 0)
			record(rep, cases.lines[i], V_WRONG, sout ? sout : "",
				rout ? rout : "");
		else
			record(rep, cases.lines[i], V_OK, NULL, NULL);
		free_tokens(args);
		free(sout);
		free(rout);
		i++;
	}
	free_cases(&cases);
	rep->verdict = verdict_of(rep);
	return (rep->verdict == V_OK ? 0 : -1);
}

/*
** Every failed attempt is written down, whatever the verdict: a missing file
** and a forbidden function are as much a part of a question's history as a
** wrong line of output, and the student can re-read all of them later.
*/
int	moulinette_run(t_session *s, t_question *q, t_report *rep)
{
	int	r;

	r = grade(s, q, rep);
	if (rep->verdict != V_OK)
		write_trace(s, q, rep);
	return (r);
}
