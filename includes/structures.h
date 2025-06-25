#ifndef STRUCTURES_H
# define STRUCTURES_H

# define HISTFILE "/.minishell_history"
# define HISTSIZE 500
# define HISTFILESIZE 500


/*
*
* t_history: History structure
*
* entries:          Inputs (limited by histfilesize)
* count:            The last index of of the session entries
* current:          The first index of of the session entries
* histmem_lines_c:  Count of in-memory history lines of the session
* histsize:         Number of lines to load from HISTFILE
* histfilesize:     Number of lines should be in HISTFILE
*
*/
typedef struct s_history
{
	char	**entries;
	int		count;
	int		current;
	int		histmem_lines_c;
	int		histsize;
	int		histfilesize;
}	t_history;


/*
*
* t_token_type: Represents type of a single string (e.g. word, pipe, ...) from the input
*
*/
typedef enum e_token_type {
	TOKEN_WORD,        // All command/argument words (after expansion/quote removal)
	TOKEN_PIPE,        // |
	TOKEN_REDIR_IN,    // <
	TOKEN_REDIR_OUT,   // >
	TOKEN_REDIR_APPEND,// >>
	TOKEN_REDIR_HEREDOC,// <<
	TOKEN_EOF
} t_token_type;

/*
*
* t_token: Token structure
*
* type:    Type of the token
* value:   The actual string content of the token
*
*/
typedef struct s_token
{
	t_token_type	type;
	char			*value;
} t_token;

typedef enum e_ast_type
{
	AST_CMD,        // Simple command (e.g., "ls -l")
	AST_PIPE,       // Pipe between commands (e.g., "cmd1 | cmd2")
	AST_REDIR,      // Redirection
}	t_ast_type;

typedef enum e_redir_type
{
	REDIR_NONE,
	REDIR_INPUT,    // <
	REDIR_OUTPUT,   // >
	REDIR_APPEND,   // >>
	REDIR_HEREDOC   // <<
}	t_redir_type;

/*
* t_ast: Represents the structured command hierarchy.
*
* Example AST(Abstract Syntax Tree for ls -l | grep "test" > out.txt:
*        PIPELINE
*        /      \
*    CMD          REDIRECT
*  (ls -l)         /    \
*                CMD     FILE
*          (grep "test")  (out.txt)
*
*
*/
typedef struct s_ast
{
	t_ast_type   type;        // Node type from enum
	char         **args;      // Command arguments (NULL-terminated array)
	struct s_ast *left;       // Left child node
	struct s_ast *right;      // Right child node

	/* Redirection-specific fields */
	char         *redir_file; // Target filename
	int          redir_fd;    // File descriptor (default: -1 for automatic)
	t_redir_type redir_type;
}	t_ast;

/*
* t_env: Environement variable wich contains:
*
* key:     e.g. "PATH"
* value:   e.g. "/usr/bin"
*/
typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

/*
*
* t_shell: Tracks everything the shell needs during execution.
*
* env:            Copy of environment variables (like PATH, USER)
* exit_status:    Tracks $? (e.g., echo $? shows the last exit code).
* is_interactive: Is the shell reading from a terminal?
* stdin_fd:       Backup of original stdin
* stdout_fd:      Backup of original stdout
*/
typedef struct s_shell
{
	t_list			*vars;
	t_list			*env_list;
	int				exit_status;
	struct termios	orig_termios;
	struct termios	new_termios;
	t_history		history;
	t_token			*tokens;
	int				is_interactive;
	int				stdin_fd;
	int				stdout_fd;
}	t_shell;


#endif
