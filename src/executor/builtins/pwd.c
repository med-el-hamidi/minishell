#include "../../../includes/minishell.h"

int	builtin_pwd(char **argv)
{
	char	*buf;

	(void)argv;
	buf = getcwd(NULL, 0);
	if (!buf)
	{
		perror("pwd");
		return (1);
	}
	ft_putendl_fd(buf, STDOUT_FILENO);
	free(buf);
	return (0);
}
