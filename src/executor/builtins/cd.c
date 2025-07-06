#include "../../../includes/minishell.h"

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*path;
	char	*pwd;
	char	*oldpwd;

	path = NULL;

	if (!argv[1])
		path = expand_env(shell->vars,"HOME");
	else if (argv[1] && argv[2])
		return (ft_putendl_fd("minishell: cd: too many arguments", STDERR_FILENO), 1);
	else if (argv[1][0] == '\0' || !ft_strcmp(argv[1], "."))
		return (0);
	else
		path = argv[1];
	oldpwd = getcwd(NULL, 0);
	if (oldpwd)
		update_shell_var(find_shell_var(shell->vars, "OLDPWD") , oldpwd, VAR_ENV);
	else
		perror("cd: error retrieving current directory");
	if (chdir(path) == -1)
	{
		free(oldpwd);
		return (perror(path), 1);
	}
	pwd = getcwd(NULL, 0);
	if (pwd)
		update_shell_var(find_shell_var(shell->vars, "PWD"), pwd, VAR_ENV);
	free(oldpwd);
	free(pwd);
	return (0);
}
