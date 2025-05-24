#include "../../../includes/minishell.h"

int builtin_cd(char **argv)
{
	char *path;

	if (!argv[1] || ft_strncmp(argv[1], "~", 1) == 0)
	{
		path = getenv("HOME");
		if(path == NULL)
		{
			ft_putstr_fd("Minishell: cd: HOME not set\n", STDERR_FILENO);
			return (1);
		}
	}
	else if (argv[1][0] == '\0')
		return (0);
	else
		path = argv[1];
	if(chdir(path) == -1)
	{
		perror("cd :");
		return 1;
	}
	return (0);
	
}