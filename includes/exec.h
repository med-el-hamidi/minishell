#ifndef EXEC_H
# define EXEC_H


int builtin_echo(char **argv);
int builtin_cd(char **argv);
int	builtin_env(t_env *env_list);
int is_builtin(char *cmd);
int	ft_strcmp(const char *s1, const char *s2);
int exec_builtins(t_ast *node, t_shell *shell);
int execute_ast(t_ast *node, t_shell *shell);

typedef struct s_env
{
	char *key;
	char *value;
	struct s_env *next;
}	t_env;

typedef struct s_llist
{
	void			*value;
	struct s_llist	*next;
}	t_llist;

typedef struct s_env_v
{
	char	*key;
	char	*val;
}	t_env_v;

#endif