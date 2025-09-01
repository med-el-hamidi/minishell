# 🐚 Minishell

A simplified Unix shell written in C, developed as part of **1337 & 42 School** curriculum by **Me** and **Oussama** (See authors bellow).  
The project reproduces the behavior of a real shell (`bash`) by implementing parsing, execution, redirections, signals, and job control — with additional custom features.  

---

## ✨ Features (Mandatory)

- **Prompt** that waits for user input.
- **History** simple history mechanism (using Up & DOWN) to get to the previous entered input.
- **Command execution** with arguments.
- **Builtins**:
  - `echo` with option `-n`
  - `cd` with relative/absolute paths
  - `pwd` with no options
  - `export` with correct sorting
  - `unset`
  - `env`
  - `exit`
- **Redirections**:
  - `<` (input)
  - `>` (output)
  - `>>` (append)
  - `<<` (heredoc)
- **Pipes**: `cmd1 | cmd2 | cmd3 ...`
- **Environment variables**: `$VAR`, `$?`
- **Signals**:
  - `Ctrl-C`: new line, redisplay prompt
  - `Ctrl-D`: exit minishell
  - `Ctrl-\`: ignored
- **Error handling** with clear messages (syntax errors, invalid commands, Ambiguous rediretion,etc.)
- **No memory leaks** (checked with `valgrind`).

---

## 🔥 Bonus Features

- Multiple pipes (`cmd1 | cmd2 | cmd3 ...`).
- Logical operators `&&` and `||`.
- Parentheses `()` for command grouping.
- Wildcards (`*`) expanded to directory contents.
- Improved signal handling (interactive vs. non-interactive mode).

---

## 🧩 Extra Custom Features

Beyond the 42 requirements, we also implemented:

- **History** (Load, Update and save history in ~/.minishell_history)
- **Local variables** (usable only in the current shell session, not exported).
- **Shell script execution with arguments**:  
  ```bash
  ./minishell script.sh arg1 arg2 arg3
Positional parameters: $0 to $9

$0 → name of the minishell or script

$1 … $9 → script arguments

Requirement note: special characters like {}, [], etc. were not implemented, as per subject instructions.

⚙️ Requirements & Restrictions (42)
Allowed functions:
malloc, free, write, access, open, read, close, stat,
fork, wait, waitpid, wait3, wait4, execve, dup, dup2, pipe,
opendir, readdir, closedir, perror, strerror, isatty, ttyname,
ttyslot, ioctl, getcwd, chdir, unlink, signal, sigaction, kill, exit
and libft functions (A static library developed at the begining of the curriculum).

🛠️ Installation & Usage
Build
```bash
git clone https://github.com/med-el-hamidi/minishell.git
cd minishell
make
```
OR ```bash make bonus ``` For the bonus part
Run interactively
```bash
./minishell
```
OR ```./minishell_bonus ``` For the bonus part

Run with script & arguments
```bash
./minishell script.sh 1 2 3
```
Inside script.sh, you can use:

```bash
echo "Script name: $0"
echo "First argument: $1"
echo "Second argument: $2"
```
📖 Examples
```bash
minishell$ echo Hello Mohamed!
Hello Mohamed!

minishell$ export NAME=World
minishell$ echo "Hello $NAME"
Hello World

minishell$ cat < input.txt | grep "42" > output.txt

minishell$ ./minishell script.sh 1 2 3
Script name: minishell
First argument: 1
Second argument: 2
```
Note: You can rename the executable file from minishell to whatever you want, and you gonna notice the prompt changed as well!

📚 What We Learned

Process management (fork, execve, waitpid)

Pipes & redirections with file descriptors

Signal handling with sigaction

Lexer & parser design

Environment vs. local variables

Script execution & argument passing

Pattern-matching algorithm with backtracking for handling wildcards (*)

Robust memory management

Team collaboration with Git/GitHub

## 👨‍💻 Authors

- [Hamidi](https://github.com/med-el-hamidi)
  *Role:* History management, Lexer, Parser design, wildcard(*)

- [Bensarjane](https://github.com/Oussama-Bensarjane)
  *Role:* Executor, redirections, pipe, &&, ||, subshells ()

⚖️ License

[![License](https://img.shields.io/badge/License-MIT-blue.svg)](https://github.com/med-el-hamidi/minishell/blob/main/LICENSE.md)
