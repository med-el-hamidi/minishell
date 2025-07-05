/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:20:08 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:20:10 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <errno.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include <sys/stat.h>
# include "../libft/libft.h"
# include "structures.h"
# include "lexer.h"
# include "parser.h"
# include "exec.h"

/* Main Functions */
void	init_shell(t_shell *shell, char **envp);
void	shell_loop(t_shell *shell);
void	cleanup_shell(t_shell *shell);

/* Error functions*/
void	exit_error(char *msg, const int sys_error);
int		print_syntax_error(const char *token);

/* Init functions*/
t_list	*init_env(char **envp);
void	setup_signals(void);
void	init_history(t_shell *shell);
void	init_termios(t_shell *shell);

/* History functions*/
void	load_history(t_shell *shell);
void	add_to_history(t_shell *shell, char *input);
void	save_history(t_shell *shell, char *path);
int		load_recent_history(char *path, t_shell *shell, int histfile_lines_c);
void	load_hist_fd(t_shell *shell, int histfile_lines_c, int *skip, int fd);
int		get_histfile_lines_c(char *path, int oflag, int perm);

/* Free functions */
void	free_2d_array(char **arr);
void	del_env(void *v);
void	del_token(void *t);
void	free_ast(t_ast *node);

/* Other utils */
void	set_herdoc_tmp_file(t_shell *shell, char **delimiter);
int		open_script(char *script);
char	*_getenv(t_shell *shell, const char *name);
char	*_getpid(void );
int		_add_history(t_shell *shell, char *line, int i, int *skip);

/*Tests functions*/
void	print_ast(t_ast *node, int depth);

#endif
