/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:25:55 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 02:20:17 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "examshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <dirent.h>

/*
** The exam workspace mimics the real thing.
**
**   github/<login>/rendu.git   a bare repository standing in for the exam
**                              server: this is what "origin" points at
**   rendu/                     the student's clone of it
**   exam/                      the subject and the traces
**
** grademe never looks at the working copy, only at what actually reached
** the server, so forgetting to commit or push fails you exactly like at 42.
**
** The workspace is deliberately created away from this checkout: an exam
** must never drop its repositories inside the examshell sources.
*/

static const char	*home_dir(void)
{
	const char		*h;
	struct passwd	*pw;

	h = getenv("HOME");
	if (h && *h)
		return (h);
	pw = getpwuid(getuid());
	if (pw)
		return (pw->pw_dir);
	return ("/tmp");
}

static int	git(const t_session *s, char *const argv[], const char *cwd)
{
	int	status;

	(void)s;
	if (run(argv, cwd, NULL, 60, &status) != 0)
		return (-1);
	return (status == 0 ? 0 : -1);
}

static const char	*login(void)
{
	const char		*u;
	struct passwd	*pw;

	u = getenv("USER");
	if (u && *u)
		return (u);
	pw = getpwuid(getuid());
	if (pw && pw->pw_name && *pw->pw_name)
		return (pw->pw_name);
	return ("student");
}

/*
** Both sides have to be resolved before they can be compared: "$PWD/../work"
** is not inside the checkout, but it starts with its name, and a plain prefix
** test refuses it.
**
** realpath() only resolves what exists, and the workspace does not yet. So
** resolve the deepest ancestor that does and hang the rest off it -- the ".."
** and the symlinks all live in that ancestor anyway. Every cut is made at a
** '/', which keeps the head a literal prefix of the path.
*/
static void	resolve(const char *path, char *out, size_t n)
{
	char	head[MAX_PATH];
	char	real[MAX_PATH];
	char	*slash;

	snprintf(head, sizeof(head), "%s", path);
	while (1)
	{
		if (realpath(head, real))
		{
			snprintf(out, n, "%s%s", real, path + strlen(head));
			return ;
		}
		slash = strrchr(head, '/');
		if (!slash || slash == head)
			break ;
		*slash = '\0';
	}
	snprintf(out, n, "%s", path);
}

/*
** Refuse to build the workspace underneath the examshell checkout, however
** EXAMSHELL_HOME was set: exam repositories have no business living in the
** simulator's own tree.
*/
static int	inside_checkout(const char *base)
{
	char	real[MAX_PATH];
	char	root[MAX_PATH];
	size_t	n;

	resolve(base, real, sizeof(real));
	resolve(es_root(), root, sizeof(root));
	n = strlen(root);
	if (n == 0 || strncmp(real, root, n) != 0)
		return (0);
	return (real[n] == '\0' || real[n] == '/');
}

int	repo_plan(t_session *s)
{
	const char	*base;
	char		stamp[64];
	time_t		now;
	struct tm	*tm;

	base = getenv("EXAMSHELL_HOME");
	now = time(NULL);
	tm = localtime(&now);
	strftime(stamp, sizeof(stamp), "%Y%m%d_%H%M%S", tm);
	if (base && *base)
		snprintf(s->workspace, sizeof(s->workspace), "%s/%s_%s", base,
			s->exam->id, stamp);
	else
		snprintf(s->workspace, sizeof(s->workspace), "%s/42_exam/%s_%s",
			home_dir(), s->exam->id, stamp);
	if (inside_checkout(s->workspace))
	{
		fprintf(stderr, C_RED "examshell: refusing to create the exam"
			" workspace inside the examshell sources (%s)." C_RESET "\n",
			s->workspace);
		fprintf(stderr, "Point EXAMSHELL_HOME somewhere else.\n");
		return (-1);
	}
	snprintf(s->server, sizeof(s->server), "%s/github/%s", s->workspace,
		login());
	snprintf(s->remote, sizeof(s->remote), "%s/rendu.git", s->server);
	snprintf(s->url, sizeof(s->url), "%s/rendu.git", login());
	snprintf(s->repo, sizeof(s->repo), "%s/rendu", s->workspace);
	snprintf(s->docs, sizeof(s->docs), "%s/exam", s->workspace);
	snprintf(s->tmp, sizeof(s->tmp), "%s/.moulinette", s->workspace);
	return (0);
}

/*
** Create what repo_plan chose: the server, the bare repository behind it,
** and the clone the student works in.
*/
int	repo_setup(t_session *s)
{
	if (mkdirs(s->workspace) != 0)
		return (-1);
	if (mkdirs(s->tmp) != 0)
		return (-1);
	if (mkdirs(s->docs) != 0)
		return (-1);
	if (mkdirs(s->server) != 0)
		return (-1);
	{
		char	*init[] = {"git", "init", "--bare", "--initial-branch=master",
			s->remote, NULL};
		char	*init_old[] = {"git", "init", "--bare", s->remote, NULL};

		if (git(s, init, NULL) != 0 && git(s, init_old, NULL) != 0)
			return (-1);
	}
	{
		char	*clone[] = {"git", "clone", "--quiet", s->remote, s->repo,
			NULL};

		if (git(s, clone, NULL) != 0)
			return (-1);
	}
	{
		char	*n[] = {"git", "config", "user.name", "student", NULL};
		char	*m[] = {"git", "config", "user.email", "student@42.fr", NULL};
		char	*b[] = {"git", "checkout", "--quiet", "-B", "master", NULL};
		char	*r[] = {"git", "config", "branch.master.remote", "origin",
			NULL};
		char	*g[] = {"git", "config", "branch.master.merge",
			"refs/heads/master", NULL};

		if (git(s, n, s->repo) != 0 || git(s, m, s->repo) != 0
			|| git(s, b, s->repo) != 0 || git(s, r, s->repo) != 0
			|| git(s, g, s->repo) != 0)
			return (-1);
	}
	{
		char	traces[MAX_PATH * 2];

		snprintf(traces, sizeof(traces), "%s/traces", s->docs);
		mkdirs(traces);
	}
	return (0);
}

/*
** Lay the paperwork out in exam/, never inside rendu/: anything sitting in
** the repository would be pushed and read back as a turned-in file. Files
** left over from the previous question are cleared, so exam/ always
** describes exactly the question in front of you.
*/
static void	place_doc(t_session *s, const char *name, char *content)
{
	char	path[MAX_PATH * 2];

	snprintf(path, sizeof(path), "%s/%s", s->docs, name);
	if (content)
	{
		write_file(path, content);
		free(content);
	}
	else
		unlink(path);
}

int	repo_arm(t_session *s)
{
	t_question	*q;
	char		*subject;

	q = session_current(s);
	if (!q)
		return (-1);
	subject = pool_subject(q->ex);
	if (!subject)
		return (-1);
	place_doc(s, "subject.txt", subject);
	place_doc(s, "subject.tr.txt", pool_subject_tr(q->ex));
	place_doc(s, "examples.txt", pool_examples(q->ex));
	pool_place_given(q->ex, s->repo);
	return (0);
}

/*
** Hand back an empty rendu for the next question: drop every tracked file
** from the working copy and force that emptiness onto the remote, so the
** previous answer can never be mistaken for the next one.
*/
int	repo_reset(t_session *s)
{
	DIR				*d;
	struct dirent	*e;
	char			child[MAX_PATH];
	char			*branch[] = {"git", "checkout", "--quiet", "-B", "master",
		NULL};

	/* Always start the next assignment on the documented default branch. */
	if (git(s, branch, s->repo) != 0)
		return (-1);

	d = opendir(s->repo);
	if (!d)
		return (-1);
	while ((e = readdir(d)))
	{
		if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")
			|| !strcmp(e->d_name, ".git"))
			continue ;
		snprintf(child, sizeof(child), "%s/%s", s->repo, e->d_name);
		rm_rf(child);
	}
	closedir(d);
	{
		char	*add[] = {"git", "add", "-A", NULL};
		char	*ci[] = {"git", "commit", "--quiet", "--allow-empty", "-m",
			"next assignment", NULL};
		char	*push[] = {"git", "push", "--quiet", "--force", "origin",
			"master", NULL};

		if (git(s, add, s->repo) != 0 || git(s, ci, s->repo) != 0
			|| git(s, push, s->repo) != 0)
			return (-1);
	}
	return (0);
}

/*
** Resolve the server ref which belongs to the branch the student currently
** has checked out. This still reads only the bare server: a local commit is
** invisible until that same branch has actually been pushed. Falling back to
** HEAD keeps detached checkouts and older workspaces working as before.
*/
static void	pushed_ref(const t_session *s, char *ref, size_t n)
{
	char	*out;
	int		status;

	snprintf(ref, n, "HEAD");
	out = NULL;
	{
		char	*argv[] = {"git", "symbolic-ref", "--quiet", "--short",
			"HEAD", NULL};

		if (run_capture(argv, s->repo, &out, 30, &status) != 0 || status != 0
			|| !out)
		{
			free(out);
			return ;
		}
	}
	out[strcspn(out, "\r\n")] = '\0';
	if (*out)
		snprintf(ref, n, "refs/heads/%s", out);
	free(out);
}

/*
** Confirm the clone really is wired to the exam server. examshell owns both
** ends, so if this ever drifts the exam is lying about what it grades.
*/
int	repo_link_ok(const t_session *s)
{
	char	*argv[] = {"git", "remote", "get-url", "origin", NULL};
	char	*out;
	int		status;
	int		ok;

	out = NULL;
	if (run_capture(argv, s->repo, &out, 30, &status) != 0 || status != 0
		|| !out)
	{
		free(out);
		return (0);
	}
	ok = (strncmp(out, s->remote, strlen(s->remote)) == 0);
	free(out);
	return (ok);
}

/*
** Ask the server what it is holding. This is the same ref grademe will be
** handed, so it doubles as the student's proof that a push landed.
*/
int	repo_pushed(const t_session *s, t_pushed *p)
{
	char	*out;
	char	ref[MAX_PATH];
	int		status;

	memset(p, 0, sizeof(*p));
	if (!repo_has_commit(s))
		return (0);
	pushed_ref(s, ref, sizeof(ref));
	{
		char	*argv[] = {"git", "--git-dir", (char *)s->remote, "log", "-1",
			"--format=%h %s", ref, NULL};

		out = NULL;
		if (run_capture(argv, NULL, &out, 30, &status) == 0 && status == 0
			&& out)
		{
			char	*sp;
			char	*nl;

			nl = strchr(out, '\n');
			if (nl)
				*nl = '\0';
			sp = strchr(out, ' ');
			if (sp)
			{
				*sp = '\0';
				snprintf(p->subject, sizeof(p->subject), "%s", sp + 1);
			}
			snprintf(p->sha, sizeof(p->sha), "%s", out);
		}
		free(out);
	}
	{
		char	*argv[] = {"git", "--git-dir", (char *)s->remote, "ls-tree",
			"--name-only", ref, NULL};

		out = NULL;
		if (run_capture(argv, NULL, &out, 30, &status) == 0 && status == 0
			&& out)
		{
			char	*c = out;

			while (*c)
				if (*c++ == '\n')
					p->files++;
		}
		free(out);
	}
	return (p->files > 0);
}

int	repo_has_commit(const t_session *s)
{
	char	ref[MAX_PATH];
	char	*argv[] = {"git", "--git-dir", (char *)s->remote, "rev-parse",
		"--verify", ref, NULL};
	int		status;

	pushed_ref(s, ref, sizeof(ref));
	if (run(argv, NULL, NULL, 30, &status) != 0)
		return (0);
	return (status == 0);
}

/*
** Export the pushed branch into `dest`. Anything sitting in the working copy
** but not pushed simply does not exist as far as this function is concerned.
*/
int	repo_collect(t_session *s, const char *dest)
{
	char	tarball[MAX_PATH];
	char	ref[MAX_PATH];
	int		status;

	if (!repo_has_commit(s))
		return (-1);
	pushed_ref(s, ref, sizeof(ref));
	rm_rf(dest);
	if (mkdirs(dest) != 0)
		return (-1);
	snprintf(tarball, sizeof(tarball), "%s/rendu.tar", s->tmp);
	{
		char	*argv[] = {"git", "--git-dir", (char *)s->remote, "archive",
			"--format=tar", "-o", tarball, ref, NULL};

		if (run(argv, NULL, NULL, 60, &status) != 0 || status != 0)
			return (-1);
	}
	{
		char	*argv[] = {"tar", "-xf", tarball, "-C", (char *)dest, NULL};

		if (run(argv, NULL, NULL, 60, &status) != 0 || status != 0)
			return (-1);
	}
	unlink(tarball);
	return (0);
}

/* ------------------------------------------------------------------ */
/* Working-copy safety                                                 */
/* ------------------------------------------------------------------ */

static int	is_source_name(const char *name)
{
	size_t	len;

	len = strlen(name);
	return (len > 2 && (!strcmp(name + len - 2, ".c")
			|| !strcmp(name + len - 2, ".h")));
}

static int	backup_sources(const char *src, const char *dst)
{
	DIR				*d;
	struct dirent	*e;
	char			from[MAX_PATH * 2];
	char			to[MAX_PATH * 2];

	d = opendir(src);
	if (!d)
		return (-1);
	if (mkdirs(dst) != 0)
	{
		closedir(d);
		return (-1);
	}
	while ((e = readdir(d)))
	{
		if (e->d_name[0] == '.')
			continue ;
		snprintf(from, sizeof(from), "%s/%s", src, e->d_name);
		snprintf(to, sizeof(to), "%s/%s", dst, e->d_name);
		if (is_dir(from))
		{
			if (backup_sources(from, to) != 0)
			{
				closedir(d);
				return (-1);
			}
		}
		else if (is_source_name(e->d_name) && copy_file(from, to) != 0)
		{
			closedir(d);
			return (-1);
		}
	}
	closedir(d);
	return (0);
}

static int	same_file(const char *a, const char *b)
{
	char	*left;
	char	*right;
	int		same;

	left = read_file(a);
	right = read_file(b);
	same = (left && right && !strcmp(left, right));
	free(left);
	free(right);
	return (same);
}

static int	restore_sources(const char *backup, const char *dst)
{
	DIR				*d;
	struct dirent	*e;
	char			from[MAX_PATH * 2];
	char			to[MAX_PATH * 2];
	int			changed;
	int			r;

	d = opendir(backup);
	if (!d)
		return (-1);
	changed = 0;
	while ((e = readdir(d)))
	{
		if (e->d_name[0] == '.')
			continue ;
		snprintf(from, sizeof(from), "%s/%s", backup, e->d_name);
		snprintf(to, sizeof(to), "%s/%s", dst, e->d_name);
		if (is_dir(from))
		{
			if (mkdirs(to) != 0)
			{
				closedir(d);
				return (-1);
			}
			r = restore_sources(from, to);
			if (r < 0)
			{
				closedir(d);
				return (-1);
			}
			changed += r;
		}
		else if (!same_file(from, to))
		{
			if (copy_file(from, to) != 0)
			{
				closedir(d);
				return (-1);
			}
			changed++;
		}
	}
	closedir(d);
	return (changed);
}

int	repo_backup_sources(const t_session *s)
{
	char	backup[MAX_PATH * 2];

	snprintf(backup, sizeof(backup), "%s/source-backup", s->tmp);
	rm_rf(backup);
	return (backup_sources(s->repo, backup));
}

int	repo_restore_sources(const t_session *s)
{
	char	backup[MAX_PATH * 2];
	int		r;

	snprintf(backup, sizeof(backup), "%s/source-backup", s->tmp);
	r = restore_sources(backup, s->repo);
	rm_rf(backup);
	return (r);
}
