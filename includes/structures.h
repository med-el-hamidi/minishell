//                     Conflict Zone
//                git pull -> instant update -> git push
//
#ifndef STRUCTURES_H
# define STRUCTURES_H

/*
*
* t_token
*
*/
typedef struct s_token {
    t_token_type   type;     // Type from the enum
    char           *value;   // The actual string content
    struct s_token *next;    // Next token in linked list
} t_token;

typedef enum e_token_type {
	TOKEN_WORD,         // Regular command/argument (e.g., "ls", "-l")
	TOKEN_PIPE,         // | (pipe operator)
	TOKEN_REDIR_IN,     // < (input redirection)
	TOKEN_REDIR_OUT,    // > (output redirection)
	TOKEN_REDIR_APPEND, // >> (append output)
	TOKEN_REDIR_HEREDOC,// << (heredoc)
	TOKEN_QUOTE,        // ' or " (quote character)
	TOKEN_DQUOTE,       // " (double quote)
	TOKEN_SQUOTE,       // ' (single quote)
	TOKEN_DOLLAR,       // $ (variable expansion)
	TOKEN_EOF,          // End of input
	TOKEN_ERROR         // Invalid token (for error handling)
}	t_token_type;

typedef enum e_ast_type {
	AST_CMD,        // Simple command (e.g., "ls -l")
	AST_PIPE,       // Pipe between commands (e.g., "cmd1 | cmd2")
	AST_REDIR_IN,   // Input redirection "<"
	AST_REDIR_OUT,  // Output redirection ">"
	AST_REDIR_APPEND, // Append output ">>"
	AST_HEREDOC,    // Heredoc "<<"
}	t_ast_type;

typedef enum e_redir_type {
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
typedef struct s_ast {
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
	char	**env;
	int		exit_status;
	t_token	*tokens;
	int		is_interactive;
	int     stdin_fd;
	int     stdout_fd;
}	t_shell;


#endif
