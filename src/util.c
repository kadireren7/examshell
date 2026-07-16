/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:26:11 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 01:26:12 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _GNU_SOURCE
#include "examshell.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

char	*es_strdup(const char *s)
{
	char	*r;

	if (!s)
		return (NULL);
	r = malloc(strlen(s) + 1);
	if (!r)
		return (NULL);
	strcpy(r, s);
	return (r);
}

/* A checkout carries both the simulator sources and the exercise data. */
static int	is_checkout(const char *dir)
{
	char	probe[MAX_PATH];
	char	source[MAX_PATH];

	if (!dir || !*dir)
		return (0);
	snprintf(probe, sizeof(probe), "%s/level0/aff_a/ref.c", dir);
	snprintf(source, sizeof(source), "%s/src", dir);
	return (path_exists(probe) && is_dir(source));
}

/*
** Locate the checkout: an explicit EXAMSHELL_ROOT wins, then the path baked
** in at build time, then the directory the running binary sits in, then the
** working directory. The last two are what keep a `git clone && make` usable
** after someone moves the folder, which the baked path alone would not
** survive.
*/
char	*es_root(void)
{
	static char	root[MAX_PATH];
	const char	*env;
	char		self[MAX_PATH];
	ssize_t		n;
	char		*slash;

	if (root[0])
		return (root);
	env = getenv("EXAMSHELL_ROOT");
	if (env && *env)
	{
		snprintf(root, sizeof(root), "%s", env);
		return (root);
	}
	if (is_checkout(EXAMSHELL_ROOT))
	{
		snprintf(root, sizeof(root), "%s", EXAMSHELL_ROOT);
		return (root);
	}
	n = readlink("/proc/self/exe", self, sizeof(self) - 1);
	if (n > 0)
	{
		self[n] = '\0';
		slash = strrchr(self, '/');
		if (slash)
			*slash = '\0';
		if (is_checkout(self))
		{
			snprintf(root, sizeof(root), "%s", self);
			return (root);
		}
	}
	if (getcwd(self, sizeof(self)) && is_checkout(self))
	{
		snprintf(root, sizeof(root), "%s", self);
		return (root);
	}
	snprintf(root, sizeof(root), "%s", EXAMSHELL_ROOT);
	return (root);
}

int	path_exists(const char *path)
{
	struct stat	st;

	return (stat(path, &st) == 0);
}

/*
** Append to a bounded buffer, leaving *o on the terminator of what fits.
**
** snprintf() reports the length it *wanted* to write, so `o += snprintf(...)`
** steps straight past the end of the buffer the first time a line is too long
** to fit -- and every write after that is out of bounds. Nothing here is
** allowed to grow the buffer, so a full one simply stops accepting.
*/
void	appendf(char *out, size_t size, size_t *o, const char *fmt, ...)
{
	va_list	ap;
	int		n;

	if (!size || *o >= size - 1)
		return ;
	va_start(ap, fmt);
	n = vsnprintf(out + *o, size - *o, fmt, ap);
	va_end(ap);
	if (n < 0)
		return ;
	if ((size_t)n >= size - *o)
		*o = size - 1;
	else
		*o += (size_t)n;
}

int	is_dir(const char *path)
{
	struct stat	st;

	return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

char	*read_file(const char *path)
{
	FILE	*f;
	char	*buf;
	long	size;
	size_t	got;

	f = fopen(path, "rb");
	if (!f)
		return (NULL);
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	if (size < 0)
	{
		fclose(f);
		return (NULL);
	}
	buf = malloc((size_t)size + 1);
	if (!buf)
	{
		fclose(f);
		return (NULL);
	}
	got = fread(buf, 1, (size_t)size, f);
	buf[got] = '\0';
	fclose(f);
	return (buf);
}

int	write_file(const char *path, const char *content)
{
	FILE	*f;

	f = fopen(path, "wb");
	if (!f)
		return (-1);
	if (content && *content)
		fwrite(content, 1, strlen(content), f);
	fclose(f);
	return (0);
}

int	mkdirs(const char *path)
{
	char	tmp[MAX_PATH];
	char	*p;

	snprintf(tmp, sizeof(tmp), "%s", path);
	for (p = tmp + 1; *p; p++)
	{
		if (*p != '/')
			continue ;
		*p = '\0';
		if (mkdir(tmp, 0755) != 0 && errno != EEXIST)
			return (-1);
		*p = '/';
	}
	if (mkdir(tmp, 0755) != 0 && errno != EEXIST)
		return (-1);
	return (0);
}

int	rm_rf(const char *path)
{
	DIR				*d;
	struct dirent	*e;
	char			child[MAX_PATH];
	struct stat		st;

	if (lstat(path, &st) != 0)
		return (0);
	if (!S_ISDIR(st.st_mode))
		return (unlink(path));
	d = opendir(path);
	if (!d)
		return (-1);
	while ((e = readdir(d)))
	{
		if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, ".."))
			continue ;
		snprintf(child, sizeof(child), "%s/%s", path, e->d_name);
		rm_rf(child);
	}
	closedir(d);
	return (rmdir(path));
}

int	copy_file(const char *src, const char *dst)
{
	char	*content;
	int		r;

	content = read_file(src);
	if (!content)
		return (-1);
	r = write_file(dst, content);
	free(content);
	return (r);
}

/*
** Fork/exec a command. stdout+stderr go to `out` (a file path) when given,
** otherwise to /dev/null. A timeout of 0 means "wait forever".
*/
static int	spawn(char *const argv[], const char *cwd, const char *out,
		pid_t *pid_out)
{
	pid_t	pid;
	int		fd;

	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0)
	{
		if (cwd && chdir(cwd) != 0)
			_exit(127);
		if (out)
			fd = open(out, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			fd = open("/dev/null", O_WRONLY);
		if (fd >= 0)
		{
			dup2(fd, STDOUT_FILENO);
			dup2(fd, STDERR_FILENO);
			close(fd);
		}
		fd = open("/dev/null", O_RDONLY);
		if (fd >= 0)
		{
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		setpgid(0, 0);
		execvp(argv[0], argv);
		_exit(127);
	}
	*pid_out = pid;
	return (0);
}

/*
** EINTR says nothing about the child, so it is waited out rather than
** reported: read as a failure it would fail a submission that compiles and
** runs perfectly well.
**
** The polling below passes WNOHANG and so does not block, which is why the
** exam shell catching ^C has not made this bite in practice. That is a
** property of the caller, not a promise, and the blocking waits here have no
** such excuse.
*/
static pid_t	wait_child(pid_t pid, int *status, int flags)
{
	pid_t	r;

	r = waitpid(pid, status, flags);
	while (r < 0 && errno == EINTR)
		r = waitpid(pid, status, flags);
	return (r);
}

static int	wait_timeout(pid_t pid, int timeout, int *status)
{
	int		elapsed;
	pid_t	r;

	if (timeout <= 0)
	{
		if (wait_child(pid, status, 0) < 0)
			return (-1);
		return (0);
	}
	elapsed = 0;
	while (elapsed < timeout * 1000)
	{
		r = wait_child(pid, status, WNOHANG);
		if (r == pid)
			return (0);
		if (r < 0)
			return (-1);
		usleep(10000);
		elapsed += 10;
	}
	kill(-pid, SIGKILL);
	kill(pid, SIGKILL);
	wait_child(pid, status, 0);
	return (1);
}

int	run(char *const argv[], const char *cwd, const char *out, int timeout,
		int *status)
{
	pid_t	pid;
	int		st;
	int		timed_out;

	st = 0;
	if (spawn(argv, cwd, out, &pid) != 0)
		return (-1);
	timed_out = wait_timeout(pid, timeout, &st);
	if (status)
		*status = st;
	return (timed_out);
}

int	run_capture(char *const argv[], const char *cwd, char **out, int timeout,
		int *status)
{
	char	tmp[MAX_PATH];
	int		r;
	int		fd;

	snprintf(tmp, sizeof(tmp), "/tmp/.es_cap_%d_%ld", (int)getpid(),
		(long)time(NULL));
	fd = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd >= 0)
		close(fd);
	r = run(argv, cwd, tmp, timeout, status);
	if (out)
		*out = read_file(tmp);
	unlink(tmp);
	return (r);
}
