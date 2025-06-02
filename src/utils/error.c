# include "../../includes/minishell.h"

/*
* exit_error: Exits from minishell with a custom msg!
*
* msg: Error msg!
* sys_error: 1 (to add system msg to the input msg)
*/
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
