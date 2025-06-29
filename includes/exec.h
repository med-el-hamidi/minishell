#ifndef EXEC_H
# define EXEC_H

#include <sys/wait.h>

/* built-ins functions*/
int		builtin_echo(char **argv);
int		builtin_cd(char **argv, t_shell *shell);
int		builtin_exit(char **argv);
int		builtin_env(t_list *env_list);
int		builtin_unset(char **argv, t_list **env_list);
int		builtin_pwd(void);
int		builtin_export(char **argv, t_list **env_list);

/* utils functions */
int		is_builtin(char *cmd);
int		ft_strcmp(const char *s1, const char *s2);
int		ft_argv_count(char **argv);
int		ft_strisnum(char *n);
char	*join_3(const char *s1, char *c, const char *s2);
char    *get_cmd_path(char *cmd, t_shell *shell);
char	**env_list_to_envp(t_list *env_list);
void	update_iterators(t_list **prev, t_list **current);
void	unset_node(t_list **env_list, t_list **current, t_list **prev);
t_list	*find_env_var(t_list *env_list, char *name);

/* executor main functions */
int		exec_builtins(t_ast *node, t_shell *shell);
int		exec_external(t_ast *node, t_shell *shell);
int		executor(t_ast *node, t_shell *shell);

/* usung parser utils functions for testing before merging*/
char	*expand_env(t_shell *shell, const char *name);

#endif