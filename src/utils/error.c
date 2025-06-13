# include "../../includes/minishell.h"

void	exit_error(char *msg, const int sys_error)
{
	ft_putstr_fd("minishell: error: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	if (sys_error)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
	}
	ft_putchar_fd('\n', STDERR_FILENO);
	exit(EXIT_FAILURE);
}
