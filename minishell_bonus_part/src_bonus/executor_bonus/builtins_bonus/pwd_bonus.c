#include "../../../includes_bonus/minishell_bonus.h"

int	builtin_pwd(t_list *vars)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
	{
		printf("%s\n", expand_env(vars, "PWD"));
		return (0);
	}
	ft_putendl_fd(buf, STDOUT_FILENO);
	free(buf);
	return (0);
}
