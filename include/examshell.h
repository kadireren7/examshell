/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   examshell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kaaltint@student.42istanbul.com.tr         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 01:26:34 by kaaltint          #+#    #+#             */
/*   Updated: 2026/07/16 06:18:09 by kaaltint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXAMSHELL_H
# define EXAMSHELL_H

# include <stddef.h>
# include <time.h>

# ifndef EXAMSHELL_ROOT
#  define EXAMSHELL_ROOT "."
# endif

# define ES_VERSION "1.2.1"

# define MAX_QUESTIONS 16
# define MAX_POOL 128
# define MAX_FILES 8
# define MAX_PATH 4096

/* ------------------------------------------------------------------ */
/* Exercise pool                                                       */
/* ------------------------------------------------------------------ */

typedef enum e_extype
{
	EX_PROGRAM,		/* student turns in a main(); compared via argv runs   */
	EX_FUNCTION	/* student turns in a function; linked with a harness  */
}	t_extype;

struct s_session;

typedef struct s_exercise
{
	const char	*name;
	int			level;
	int			difficulty;		/* rank inside its own level: 1..4     */
	t_extype	type;
	const char	*files[MAX_FILES];	/* expected files, NULL terminated    */
	const char	*allowed;			/* human readable allowed functions   */
	const char	*syms[MAX_FILES];	/* symbols the linker may resolve     */
	const char	*given[MAX_FILES];	/* headers the exam provides, not the
									   student: dropped into the rendu at
									   hand-out and forced back over theirs
									   at grading time                     */
}	t_exercise;

const t_exercise	*pool_all(size_t *count);
const t_exercise	*pool_find(const char *name);
int					pool_pick(int level, int difficulty, const char **used,
						size_t used_n, const t_exercise **out);

/* Assignments drawn by recent sessions, so a new one feels new. */
# define MAX_HISTORY 48

typedef struct s_history
{
	char	name[MAX_HISTORY][64];
	size_t	n;
}	t_history;

void				history_load(t_history *h);
void				history_save(const t_history *h, const struct s_session *s);
char				*pool_subject(const t_exercise *ex);
char				*pool_examples(const t_exercise *ex);
char				*pool_subject_tr(const t_exercise *ex);
int					pool_place_given(const t_exercise *ex, const char *dir);

/* ------------------------------------------------------------------ */
/* Exam definitions                                                    */
/* ------------------------------------------------------------------ */

typedef struct s_slot
{
	int			level;
	int			points;
	int			difficulty;	/* preferred rank inside this level: 1..4 */
	const char	*exercise;	/* fixed assignment, or NULL for a random draw */
}	t_slot;

typedef struct s_exam
{
	const char	*id;
	const char	*label;
	const char	*blurb;
	int			duration;			/* seconds                            */
	int			pass;				/* points needed to validate          */
	size_t		nslots;
	t_slot		slots[MAX_QUESTIONS];
}	t_exam;

const t_exam	*exam_all(size_t *count);
const t_exam	*exam_by_index(size_t i);

/* ------------------------------------------------------------------ */
/* Session                                                             */
/* ------------------------------------------------------------------ */

typedef enum e_qstate
{
	Q_PENDING,
	Q_PASSED,
	Q_SKIPPED
}	t_qstate;

typedef struct s_question
{
	const t_exercise	*ex;
	int					points;
	int					attempts;
	t_qstate			state;
}	t_question;

typedef struct s_session
{
	const t_exam	*exam;
	t_question		q[MAX_QUESTIONS];
	size_t			nq;
	size_t			cur;
	int				grade;
	time_t			start;
	time_t			deadline;
	int				finished;
	char			workspace[MAX_PATH];
	char			repo[MAX_PATH];		/* rendu/: the student's clone    */
	char			docs[MAX_PATH];		/* exam/: subjects and traces     */
	char			server[MAX_PATH];	/* github/: the exam server       */
	char			remote[MAX_PATH];	/* the bare repo inside it        */
	char			url[256];			/* how that remote is addressed   */
	char			tmp[MAX_PATH];		/* scratch for grading            */
}	t_session;

int		session_start(t_session *s, const t_exam *exam);
int		session_plan_questions(t_session *s, const t_history *history);
void	session_next(t_session *s);
void	session_finish(t_session *s, int by_user);
int		session_remaining(const t_session *s);
t_question	*session_current(t_session *s);

/* ------------------------------------------------------------------ */
/* Repository plumbing (the "github style" bit)                        */
/* ------------------------------------------------------------------ */

/*
** repo_plan decides where the workspace goes and refuses a location inside
** the checkout. It must run before anything else writes to disk, so that a
** refusal leaves nothing behind. repo_setup then builds what it chose.
*/
int		repo_plan(t_session *s);
int		repo_setup(t_session *s);
int		repo_arm(t_session *s);					/* stage subject for a question */
int		repo_reset(t_session *s);				/* wipe rendu between questions */
int		repo_collect(t_session *s, const char *dest);	/* export pushed ref */
int		repo_has_commit(const t_session *s);
int		repo_backup_sources(const t_session *s);
int		repo_restore_sources(const t_session *s);

/*
** What the exam server currently holds: the short sha and subject of the
** pushed branch, and how many files came with it. Empty sha means nothing has
** been pushed for this question yet.
*/
typedef struct s_pushed
{
	char	sha[16];
	char	subject[128];
	int		files;
}	t_pushed;

int		repo_pushed(const t_session *s, t_pushed *p);
int		repo_link_ok(const t_session *s);

/* ------------------------------------------------------------------ */
/* Moulinette                                                          */
/* ------------------------------------------------------------------ */

typedef enum e_verdict
{
	V_OK,
	V_NO_COMMIT,
	V_MISSING_FILE,
	V_FORBIDDEN,
	V_COMPILE,
	V_WRONG,
	V_TIMEOUT,
	V_CRASH,
	V_INTERNAL
}	t_verdict;

# define MAX_CASES 64

/*
** One line of the test log. It deliberately records what the submission did
** and never what it should have done: the log tells you which case broke,
** not how to fix it.
*/
typedef struct s_case_result
{
	char		args[256];
	t_verdict	verdict;
	int			ran;
	int			diff_line;	/* 1-based line where the output first differs */
	char		got[256];
}	t_case_result;

typedef struct s_report
{
	t_verdict		verdict;
	char			detail[1024];
	char			trace[MAX_PATH];
	int				total;
	int				passed;
	t_case_result	cases[MAX_CASES];
	int				ncases;
}	t_report;

int		moulinette_run(t_session *s, t_question *q, t_report *rep);
const char	*verdict_str(t_verdict v);

/* ------------------------------------------------------------------ */
/* Shell                                                               */
/* ------------------------------------------------------------------ */

int		shell_loop(t_session *s);

/* Grade the solutions archived in the level directories. */
int		selftest(int only_level);

/* ------------------------------------------------------------------ */
/* UI                                                                  */
/* ------------------------------------------------------------------ */

# define C_RESET	"\033[0m"
# define C_BOLD		"\033[1m"
# define C_DIM		"\033[2m"
# define C_RED		"\033[31m"
# define C_GREEN	"\033[32m"
# define C_YELLOW	"\033[33m"
# define C_BLUE		"\033[34m"
# define C_MAGENTA	"\033[35m"
# define C_CYAN		"\033[36m"
# define C_GREY		"\033[90m"

# define RULE "\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500\u2500"

void	ui_banner(void);
void	ui_rule(void);
void	ui_menu(void);
void	ui_exam_header(const t_session *s);
void	ui_question(const t_session *s);
void	ui_report(const t_session *s, const t_report *rep);
void	ui_final(const t_session *s);
void	ui_fmt_time(int seconds, char *out, size_t n);

/* A path with $HOME folded back into "~", the way a shell shows it. */
const char	*ui_short(const char *path, char *out, size_t n);

/* ------------------------------------------------------------------ */
/* Util                                                                */
/* ------------------------------------------------------------------ */

char	*read_file(const char *path);
int		write_file(const char *path, const char *content);
int		mkdirs(const char *path);
int		rm_rf(const char *path);
int		copy_file(const char *src, const char *dst);
int		path_exists(const char *path);
int		is_dir(const char *path);
int		run(char *const argv[], const char *cwd, const char *out,
			int timeout, int *status);
int		run_capture(char *const argv[], const char *cwd, char **out,
			int timeout, int *status);
char	*es_strdup(const char *s);
char	*es_root(void);

/*
** Append to a bounded buffer, advancing *o by what actually fit. Buffers that
** fill up stop accepting rather than overrun.
*/
void	appendf(char *out, size_t size, size_t *o, const char *fmt, ...)
		__attribute__((format(printf, 4, 5)));

#endif
