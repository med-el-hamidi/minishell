#include "../../includes/minishell.h"

static void	*cleanup_init_shell(t_list	**head_env)
{
	ft_lstclear(head_env, del_env);
	return (NULL);
}

static t_env	*create_env_v(char *str)
{
	t_env	*v;

	if (!str)
		return (NULL);
	v = malloc(sizeof(t_env));
	if (!v)
		return (NULL);
	v->key = ft_substr(str, 0, ft_strchr(str, '=') - str);
	if (ft_strchr(str, '=') + 1)
		v->value = ft_strdup(ft_strchr(str, '=') + 1);
	else
		v->value = NULL;
	return (v);
}

t_list	*init_env(char **envp)
{
	t_list	*head_env;
	t_list	*node_env;
	t_env	*v;
	int		i;

	if (!envp || !*envp)
		return (NULL);
	head_env = NULL;
	i = -1;
	while (envp[++i])
	{
		v = create_env_v(envp[i]);
		if (!v)
			cleanup_init_shell(&head_env);
		node_env = ft_lstnew(v);
		if (!node_env)
			cleanup_init_shell(&head_env);
		ft_lstadd_back(&head_env, node_env);
	}
	// Increment SHLVL if it exists
	//increment_shell_level(head_env);
	return (head_env);
}

void	init_history(t_shell *shell)
{
	char	*path;
	char	*val;
	int		n;

	shell->history.count = 0;
	shell->history.current = 0;
	shell->history.histmem_lines_c = 0;
	n = 0;
	val = getenv("HISTSIZE");
	if (val)
		n = ft_atoi(val);
	if (n <= 0)
		n = HISTSIZE;
	shell->history.histsize = n;
	n = 0;
	val = getenv("HISTFILESIZE");
	if (val)
		n = ft_atoi(val);
	if (n <= 0)
		n = HISTFILESIZE;
	shell->history.histfilesize = n;
	shell->history.entries = malloc((shell->history.histfilesize + 1) * sizeof(char *));
	if (!shell->history.entries)
	{
		perror("minishell: history is not initialized!");
		return ;
	}
	ft_bzero(shell->history.entries, (shell->history.histfilesize + 1) * sizeof(char *));
	path = get_history_path(shell);
	if (!path)
		return ;
	if (!access(path, F_OK | R_OK))
		load_history(shell);
	free(path);
}

void	init_termios(t_shell *shell)
{
	tcgetattr(STDIN_FILENO, &shell->orig_termios);
	shell->new_termios = shell->orig_termios;
	shell->new_termios.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->new_termios);
}
