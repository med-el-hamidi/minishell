/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:20:15 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:20:16 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# define HISTFILE "/.minishell_history"
# define HISTSIZE 500
# define HISTFILESIZE 500

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

typedef enum e_token_type {
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_EOF
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

typedef struct s_env
{
	char	*key;
	char	*value;
}	t_env;

typedef struct s_shell
{
	t_list			*vars;
	t_list			*env_list;
	int				exit_status;
	struct termios	orig_termios;
	struct termios	new_termios;
	t_history		history;
	int				is_interactive;
	int				stdin_fd;
	int				stdout_fd;
}	t_shell;
#endif
