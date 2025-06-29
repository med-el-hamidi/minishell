#include "../../../includes/minishell.h"

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*path;

	path = NULL;
	if (!argv[1])
		path = expand_env(shell,"HOME");
	else if (argv[1][0] == '\0')
		return (0);
	else
		path = argv[1];
	if (chdir(path) == -1)
	{
		perror("cd :");
		return (1);
	}
	return (0);
}
