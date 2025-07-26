/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:20:15 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 21:31:47 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_BONUS_H
# define STRUCTURES_BONUS_H

# define HISTFILE "/.minishell_history"
# define HISTSIZE 500
# define HISTFILESIZE 500
# define CHUNK_SIZE 512

typedef struct s_history
{
	char	**entries;
	char	*path;
	int		count;
	int		current;
	int		histmem_lines_c;
	int		histsize;
	int		histfilesize;
}	t_history;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_P_OPEN,
	TOKEN_P_CLOSE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_AMB_REDIR
}	t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}	t_token;

typedef enum e_ast_type
{
	AST_CMD,
	AST_PIPE,
	AST_REDIR,
	AST_AND,
	AST_OR,
}	t_ast_type;

typedef enum e_redir_type
{
	REDIR_NONE,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;

typedef struct s_ast
{
	t_ast_type		type;
	char			**args;
	struct s_ast	*left;
	struct s_ast	*right;
	char			*redir_file;
	int				redir_fd;
	t_redir_type	redir_type;
}	t_ast;

typedef enum e_var_type
{
	VAR_LOCAL,
	VAR_EXPORTED,
	VAR_ENV
}	t_var_type;

typedef struct s_var
{
	char		*key;
	char		*value;
	t_var_type	flag;
}	t_var;

typedef struct s_shell
{
	t_list			*vars;
	int				exit_status;
	struct termios	orig_termios;
	struct termios	new_termios;
	t_history		history;
	t_list			**tokens;
	char			*name;
	char			*prompt;
	char			**argv;
	int				argc;
	int				is_interactive;
	int				stdin_fd;
	int				stdout_fd;
}	t_shell;

typedef struct s_lexerctx
{
	t_shell	*shell;
	t_list	**tokens;
	char	*input;
	size_t	*i;
	int		amb;
	int		f;
}	t_lexerctx;

typedef struct s_glob
{
	DIR				*dir;
	struct dirent	*entry;
	char			*h_name;
	char			*t_name;
	char			*dir_path;
	char			*ptr;
	char			*pattern;
	int				f;
	int				must_be_dir;
}	t_glob;
#endif
