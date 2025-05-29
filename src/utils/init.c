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
	char	*val;
	int		n;

	shell->history.count = 0;
	shell->history.current = 0;
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
		exit_error("History is not initialized!", 1);
	ft_bzero(shell->history.entries, (shell->history.histfilesize + 1) * sizeof(char *));
	load_history(shell);
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
