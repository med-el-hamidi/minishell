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
