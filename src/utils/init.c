# include "../../includes/minishell.h"

static void	*cleanup_init_shell(t_list	**head_env, char **split)
{
	free_2d_array(split);
	ft_lstclear(head_env, del_env);
	return (NULL);
}

static t_env	*create_env_v(char **split)
{
	t_env *v;

	v = malloc(sizeof(t_env));
	if (!v)
		return (NULL);
	v->key = ft_strdup(split[0]);
	v->value = ft_strdup(split[1]);
	return (v);
}

t_list	*init_env(char **envp)
{
	t_list	*head_env;
	t_list	*node_env;
	t_env	*v;
	char	**split;
	int		i;

	if (!envp || !*envp)
		return (NULL);
	head_env = NULL;
	i = -1;
	while (envp[++i])
	{
		split = ft_split(envp[i], '=');
		if (!split || !*split)
			cleanup_init_shell(&head_env, NULL);
		v = create_env_v(split);
		if (!v)
			cleanup_init_shell(&head_env, split);
		node_env = ft_lstnew(v);
		if (!node_env)
			cleanup_init_shell(&head_env, split);
		free_2d_array(split);
		ft_lstadd_back(&head_env, node_env);
	}
	// Increment SHLVL if it exists
	//increment_shell_level(head_env);
	return (head_env);
}

/*
*
* init_history: Initialize history of minishell
*
* 1. Read existing history
* 2. Set history file for future writes
*
*/
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
	path = get_history_path();
	if (!path)
		return ;
	if (!access(path, F_OK | R_OK))
		load_history(shell);
	free(path);
}

void	init_termios(t_shell *shell)
{
    // Backup original terminal settings
	tcgetattr(STDIN_FILENO, &shell->orig_termios);

    // Configure new settings (e.g., disable echo control chars)
	shell->new_termios = shell->orig_termios;
	shell->new_termios.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->new_termios);
}
