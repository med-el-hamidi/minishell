#include "../../../includes/minishell.h"

static int	update_the_env(char *key, char *path, t_list **vars)
{
	t_list	*existing;
	t_var	*add_node;

	existing = find_env_var(*vars, key);
	if (existing)
	{
		free(((t_var *)existing->content)->value);
		((t_var *)existing->content)->value = ft_strdup(path);
	}
	else
	{
		add_node = malloc(sizeof(t_var));
		if (!add_node)
			return (1);
		add_node->key = ft_strdup(key);
		add_node->value = ft_strdup(path);
		ft_lstadd_back(vars, ft_lstnew(add_node));
	}
	return (0);
}

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
		update_the_env("OLDPWD", oldpwd, &shell->vars);
	if (chdir(path) == -1)
	{
		free(oldpwd);
		return (perror("Minishell: cd"), 1);
	}
	pwd = getcwd(NULL, 0);
	if (pwd)
		update_the_env("PWD", pwd, &shell->vars);
	free(oldpwd);
	free(pwd);
	return (0);
}
