#include "../../includes/minishell.h"

static t_var	*create_env_v(char *str)
{
	t_var	*v;

	if (!str)
		return (NULL);
	v = malloc(sizeof(t_var));
	if (!v)
		return (NULL);
	v->key = ft_substr(str, 0, ft_strchr(str, '=') - str);
	if (ft_strchr(str, '=') + 1)
		v->value = ft_strdup(ft_strchr(str, '=') + 1);
	else
		v->value = NULL;
	v->flag = VAR_ENV;
	return (v);
}

t_list	*init_env(char *name, char **envp)
{
	t_list	*head_env;
	t_list	*node_env;
	t_var	*v;
	int		i;

	head_env = NULL;
	if (envp)
	{
		i = -1;
		while (envp[++i])
		{
			v = create_env_v(envp[i]);
			if (!v)
				return (ft_lstclear(&head_env, del_env), NULL);
			node_env = ft_lstnew(v);
			if (!node_env)
				return (ft_lstclear(&head_env, del_env), NULL);
			ft_lstadd_back(&head_env, node_env);
		}
	}
	init_shell_vars(name, &head_env);
	return (head_env);
}

static void	_set_history_sizes(t_shell *shell)
{
	char	*val;
	int		n;

	n = 0;
	val = getenv("HISTSIZE");
	if (val)
		n = ft_atoi(val);
	if (n <= 0)
	{
		n = HISTSIZE;
		set_default_history_sizes(&shell->vars, "HISTSIZE", n);
	}
	shell->history.histsize = n;
	n = 0;
	val = getenv("HISTFILESIZE");
	if (val)
		n = ft_atoi(val);
	if (n <= 0)
	{
		n = HISTFILESIZE;
		set_default_history_sizes(&shell->vars, "HISTFILESIZE", n);
	}
	shell->history.histfilesize = n;
}

void	init_history(t_shell *shell)
{
	shell->history.count = 0;
	shell->history.current = 0;
	shell->history.histmem_lines_c = 0;
	_set_history_sizes(shell);
	shell->history.entries = \
			malloc((shell->history.histfilesize + 1) * sizeof(char *));
	if (!shell->history.entries)
	{
		perror("minishell: history is not initialized!");
		return ;
	}
	ft_bzero(shell->history.entries,
		(shell->history.histfilesize + 1) * sizeof(char *));
	set_histfile(shell);
	if (!shell->history.path || !*shell->history.path)
		return ;
	if (!access(shell->history.path, F_OK | R_OK))
		load_history(shell);
}

void	init_termios(t_shell *shell)
{
	tcgetattr(STDIN_FILENO, &shell->orig_termios);
	shell->new_termios = shell->orig_termios;
	shell->new_termios.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->new_termios);
}
