#include "../../../includes/minishell.h"

int buildin_echo(char **argv)
{
	int i = 1;
	int new_line = 1;

	while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		new_line = 0;
		i++;
	}

	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (new_line)
		ft_putstr_fd("\n", 1);
	return 0;
}
