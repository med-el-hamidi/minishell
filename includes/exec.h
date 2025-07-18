/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 15:27:21 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/18 03:12:38 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include <sys/wait.h>

# define CD_ARGS "minishell: cd: too many arguments"
# define CD_RETRIV "minishell: cd: error retrieving current directory"


typedef struct s_int_f
{
	int	nbr;
	int	f;
}	t_int_f;

/* built-ins functions*/
int		builtin_echo(char **argv);
int		builtin_cd(char **argv, t_shell *shell);
void	builtin_exit(t_ast *ast, t_shell *sh);
int		builtin_env(t_list *vars);
int		builtin_unset(char **argv, t_list **vars);
int		builtin_pwd(t_list *vars);
int		builtin_export(char **argv, t_list **vars);

/* utils functions */
int		is_builtin(char *cmd);
int		_is_local_vars(t_ast *node);
int		ft_strcmp(const char *s1, const char *s2);
size_t	count_2d_array(char **arr);
int		ft_strisnum(char *n);
char	*join_3(const char *s1, char *c, const char *s2);
char	*get_cmd_path(char *cmd, t_shell *shell);
char	**env_list_to_envp(t_list *vars);
void	update_iterators(t_list **prev, t_list **current);
void	unset_node(t_list **vars, t_list **current, t_list **prev);
t_list	*find_shell_var(t_list *vars, char *key);
int		_export(t_list *vars);
void	update_shell_var(t_list *node, char *value, t_var_type flag);
void	create_shell_var(t_list **vars, char *key, \
char	*value, t_var_type flag);
int		is_directory(char *path);
int		is_regular_file(char *path);
void	_add_or_update_path_cd(t_list **vars, char *name, char *value);

/* executor main functions */
int		exec_redirection(t_ast *node, t_shell *shell);
int		exec_pipe(t_ast *node, t_shell *shell);
int		exec_local_vars(char **args, t_list **vars);
int		exec_builtins(t_ast *node, t_shell *shell);
int		exec_external(t_ast *node, t_shell *shell);
int		executor(t_ast *node, t_shell *shell);

/* print fcts error */
int		export_print_error(char *arg);
void	pip_error_exit_child(const char *msg);
int		execv_print_error(char *arg, int fg);

/* using parser utils functions for testing before merging*/
char	*expand_env(t_list *vars, const char *name);

#endif