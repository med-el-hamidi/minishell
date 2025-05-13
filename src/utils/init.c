# include "../../includes/minishell.h"

char	**copy_env(char **envp)
{
	char	**new_env;
	int		i;
	int		count;

	if (!envp || !*envp)
		return (NULL);
	count = 0;
	while (envp[count])
		count++;
	new_env = (char **)malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			free_2d_array(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	// Increment SHLVL if it exists
	//increment_shell_level(new_env);
	return (new_env);
}

static void handle_sigint(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	//g_exit_status = 130; // Set exit status (128 + SIGINT=2)
}

void	setup_signals(void)
{
	struct sigaction	sa;

	// Ignore Ctrl+\ (SIGQUIT)
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGQUIT, &sa, NULL);
	// Handle Ctrl+C (SIGINT)
	sa.sa_handler = handle_sigint;
	sigaction(SIGINT, &sa, NULL);

	// Handle EOF (Ctrl+D)
}
