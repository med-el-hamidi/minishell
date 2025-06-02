#ifndef EXEC_H
# define EXEC_H

/* built-ins functions*/
int builtin_echo(char **argv);
int builtin_cd(char **argv);
int	builtin_env(t_list *env_list);
int	builtin_unset(char **argv, t_list *env_list);
int	builtin_export(char **argv, t_list **env_list);

/* utils functions */
int is_builtin(char *cmd);
int	ft_strcmp(const char *s1, const char *s2);
int ft_argv_count(char **argv);
t_list	*find_env_var(t_list *env_list, char *name);

/* executor main functions */
int exec_builtins(t_ast *node, t_shell *shell);
int executor(t_ast *node, t_shell *shell);

#endif