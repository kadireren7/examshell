# ExamShell

ExamShell is a terminal-based 42 Piscine exam simulator written entirely in C.
It creates an isolated exam workspace, serves questions by difficulty, grades
only the code pushed to the local exam repository, and produces detailed test
reports without revealing the expected answer.

The project, its simulator, exercise pool, subjects, reference solutions,
tests, translations, and documentation were created by **kaaltint**. No
third-party source code or external content is used in this repository.

## Features

- Four exam modes: `exam00`, `exam01`, `exam02`, and `final`
- 73 exercises across six difficulty levels
- English and Turkish subject files
- Real Git-based submission flow with a local bare repository
- Compilation with `-Wall -Wextra -Werror`
- Checks for required files, allowed functions, and allowed headers
- Recursive discovery of submitted `.c` and `.h` files
- Output comparison against private reference implementations
- Per-attempt traces and complete grading history
- Automatic restoration of source files after grading
- Recent-question history to reduce repeated exercises
- Guaranteed Level 0 start in every exam mode
- Built-in end-to-end self-test suite

## Requirements

- A POSIX-compatible system
- A C compiler available as `cc`
- `make`
- `git`
- `nm` from binutils

## Installation

```sh
git clone https://github.com/kadireren7/examshell.git
cd examshell
make install
```

`make install` builds the project and installs the `42-examshell` launcher in
`~/.local/bin`. The separate name is intentional: it never shadows 42's real
`examshell` command. This installation step is required before first use.

If `~/.local/bin` is not already in your `PATH`, add it to your shell
configuration:

```sh
export PATH="$HOME/.local/bin:$PATH"
```

Then start ExamShell from anywhere:

```sh
42-examshell
```

Keep the cloned repository in place after installation because the launcher
uses its exercise data. The installation can be removed with:

```sh
make uninstall
```

## Usage

Start the interactive exam menu:

```sh
42-examshell
```

Start a specific exam directly:

```sh
42-examshell exam00
42-examshell exam01
42-examshell exam02
42-examshell final
```

The menu also accepts the exam number or exam identifier. Enter `q`, `quit`,
or `exit` to close it.

## Exam modes

| Exam | Questions | Duration | Difficulty | Passing score |
|---|---:|---:|---|---:|
| Exam 00 | 10 | 4 hours | Introductory | 30/100 |
| Exam 01 | 10 | 4 hours | Intermediate | 30/100 |
| Exam 02 | 10 | 4 hours | Advanced | 30/100 |
| Final Exam | 14 | 8 hours | Full pool | 30/100 |

Exam 00, Exam 01, and Exam 02 award 10 points per question. The Final Exam
awards 6 points for its first ten questions and 10 points for its last four.
A failed submission does not remove points; the same question remains active
until it passes or the exam ends.

Every exam always begins at Level 0. Question selection stays within the
level defined by the exam plan. When recent-session history has exhausted a
level, ExamShell reuses an older exercise from that same level instead of
silently jumping to another one.

## Workspace and submission flow

Each session creates a timestamped workspace under `~/42_exam` by default:

```text
~/42_exam/exam00_YYYYMMDD_HHMMSS/
├── rendu/                    # your working Git repository
├── exam/
│   ├── subject.txt           # English subject
│   ├── subject.tr.txt        # Turkish subject
│   ├── examples.txt          # optional examples
│   └── traces/               # grading reports
└── github/<login>/rendu.git  # local exam server
```

Set `EXAMSHELL_HOME` to store sessions elsewhere. Set `EXAMSHELL_ROOT` only
when ExamShell must use exercise data from a different checkout.

Write the requested files in `rendu/`, then commit and push them:

```sh
cd ~/42_exam/exam00_*/rendu
git add .
git commit -m "solve exercise"
git push
```

Return to ExamShell and run:

```text
grademe
```

Only the commit currently pushed to the exam server is graded. Uncommitted or
unpushed changes are intentionally ignored.

## Exam commands

| Command | Description |
|---|---|
| `grademe` | Grades the latest pushed commit |
| `logs` | Displays the grading history for the current question |
| `history` | Alias for `logs` |
| `finish` | Ends the exam and prints the final result |

## Grading rules

A submission passes only when every check succeeds:

1. The expected files exist in the pushed commit.
2. The submitted source compiles with `-Wall -Wextra -Werror`.
3. The code uses only the functions allowed by the subject.
4. Included system headers match the permitted functions and types.
5. Every test finishes without a crash or timeout.
6. The produced output matches the reference output byte for byte.

Submitted files may be placed at the repository root or in nested
directories, but their names must match the subject exactly. When a subject
provides a header, ExamShell installs its own copy before compiling.

The grader reports each executed case and shows the submission's output when
a case fails. Expected output remains hidden. Every attempt is also saved in
the session's `exam/traces/` directory.

Before grading, ExamShell backs up all `.c` and `.h` files in the working copy.
If grading changes, removes, or truncates one of them, the original file is
restored automatically.

## Exercise pool

The repository contains 73 exercises:

| Level | Exercise count | Main topics |
|---:|---:|---|
| 0 | 8 | Basic output and arguments |
| 1 | 15 | Loops, strings, and simple functions |
| 2 | 15 | Memory, bits, and string operations |
| 3 | 15 | Parsing, ranges, arithmetic, and formatting |
| 4 | 11 | Lists, allocation, sorting, and traversal |
| 5 | 9 | Advanced parsing, algorithms, and state machines |

Each exercise directory contains an English subject, a Turkish subject, a
private reference implementation, and either argument cases or a test
harness. Some exercises additionally provide required headers or examples.

> [!WARNING]
> Files named `ref.c` are used by the grader as reference implementations.
> Reading them before solving an exercise defeats the purpose of the simulator.

## Project structure

```text
.
├── include/examshell.h       # shared declarations and data structures
├── src/                      # simulator implementation
├── level0/ ... level5/       # subjects, tests, and references
├── Makefile                  # build and installation targets
└── LICENSE                   # license terms
```

The main modules are separated by responsibility:

- `session.c` selects questions and controls exam progress.
- `repo.c` creates the local server and manages pushed submissions.
- `moulinette.c` compiles, validates, executes, and compares answers.
- `pool.c` defines the exercise pool and its metadata.
- `shell.c` handles exam commands.
- `ui.c` renders terminal output.
- `selftest.c` verifies the complete grading pipeline.

## Adding an exercise

Create `level<N>/<exercise>/` and add:

1. `subject.en.txt` for the English subject.
2. `subject.tr.txt` for the Turkish subject.
3. `ref.c` for the private reference implementation.
4. `args.txt` for program arguments or `harness.c` for function exercises.
5. Optional `examples.txt`, `given/`, or `turnin/` content when required.
6. A matching entry in `g_pool` inside `src/pool.c`.

An exercise without `ref.c` is excluded from selection.

## Self-test

Run the complete test suite:

```sh
42-examshell --selftest
```

Run only one level:

```sh
42-examshell --selftest 2
```

The self-test submits every reference answer through the same Git and grading
path used by a normal session. It also verifies expected failures such as
missing files, forbidden functions, compiler errors, and wrong output.

## Make targets

| Target | Action |
|---|---|
| `make` | Builds `42-examshell` |
| `make clean` | Removes object files |
| `make fclean` | Removes object files and the executable |
| `make re` | Performs a clean rebuild |
| `make install` | Installs the user launcher |
| `make uninstall` | Removes the user launcher |

## Author

Created and maintained by **kaaltint**.

Every C source and header file carries the `kaaltint` 42 header.

## License

This project is released under the terms in [LICENSE](LICENSE).
