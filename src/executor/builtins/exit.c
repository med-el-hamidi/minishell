#include "../../../includes/minishell.h"

int	builtin_exit(char **argv, t_shell *sh)
{
	int	should_exit;
	int	n;

	should_exit = 1;
	sh->exit_status = 0;
	ft_putendl_fd("exit", STDERR_FILENO);
	if (argv[1] && argv[2])
	{
		sh->exit_status = 1;
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		should_exit = 0;
	}
	else if (argv[1] && ft_strisnum(argv[1]) == 0)
	{
		sh->exit_status = 255;
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
	}
	else if (argv[1])
	{
		n = ft_atoi(argv[1]);
		sh->exit_status = (int)(n & 0xFF);
	}
	return (should_exit);
}