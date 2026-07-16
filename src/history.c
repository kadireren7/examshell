/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:25:46 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 01:25:47 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "examshell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

/*
** A short memory of the assignments recent sessions handed out, kept in
** ~/.examshell/history. Drawing a new exam prefers anything not in here, so
** sitting exam00 twice in a row does not serve up the same ten questions.
** It is only a preference: when a level has nothing fresh left, the history
** is ignored for that slot rather than leaving it empty.
*/

static const char	*base_dir(void)
{
	static char		dir[MAX_PATH];
	const char		*env;
	const char		*home;
	struct passwd	*pw;

	if (dir[0])
		return (dir);
	env = getenv("EXAMSHELL_HOME");
	if (env && *env)
	{
		snprintf(dir, sizeof(dir), "%s", env);
		return (dir);
	}
	home = getenv("HOME");
	if (!home || !*home)
	{
		pw = getpwuid(getuid());
		home = pw ? pw->pw_dir : "/tmp";
	}
	snprintf(dir, sizeof(dir), "%s/examshell", home);
	return (dir);
}

static void	history_path(char *out, size_t n)
{
	snprintf(out, n, "%s/history", base_dir());
}

void	history_load(t_history *h)
{
	char	path[MAX_PATH * 2];
	char	*data;
	char	*line;
	char	*save;

	h->n = 0;
	history_path(path, sizeof(path));
	data = read_file(path);
	if (!data)
		return ;
	line = strtok_r(data, "\n", &save);
	while (line && h->n < MAX_HISTORY)
	{
		if (*line)
			snprintf(h->name[h->n++], sizeof(h->name[0]), "%s", line);
		line = strtok_r(NULL, "\n", &save);
	}
	free(data);
}

/*
** Write this session's questions to the front of the history and keep the
** tail of the old one, up to the cap.
*/
void	history_save(const t_history *h, const struct s_session *s)
{
	char	path[MAX_PATH * 2];
	FILE	*f;
	size_t	i;
	size_t	written;

	mkdirs(base_dir());
	history_path(path, sizeof(path));
	f = fopen(path, "w");
	if (!f)
		return ;
	written = 0;
	i = 0;
	while (i < s->nq && written < MAX_HISTORY)
	{
		fprintf(f, "%s\n", s->q[i].ex->name);
		written++;
		i++;
	}
	i = 0;
	while (i < h->n && written < MAX_HISTORY)
	{
		size_t	j;
		int		dup;

		dup = 0;
		j = 0;
		while (j < s->nq)
			if (!strcmp(h->name[i], s->q[j++].ex->name))
				dup = 1;
		if (!dup)
		{
			fprintf(f, "%s\n", h->name[i]);
			written++;
		}
		i++;
	}
	fclose(f);
}
