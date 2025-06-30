#include "../../../includes/minishell.h"

int	builtin_echo(char **argv)
{
	int	i;
	int	j;
	int	new_line;

	i = 1;
	new_line = 1;
	while (argv[i] && ft_strncmp(argv[i], "-n", 2) == 0)
	{
		j = 2;
		while (argv[i][j] == 'n')
			j++;
		if (!argv[i][j])
		{
			new_line = 0;
			i++;
		}
		else
			break ;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (new_line)
		ft_putchar_fd('\n', 1);
	return (0);
}
