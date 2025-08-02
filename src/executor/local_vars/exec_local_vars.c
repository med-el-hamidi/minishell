#include "../../../includes/minishell.h"

static void	_set_shell_var(t_list **vars, char *args, char *value)
{
	t_list	*node;

	node = find_shell_var(*vars, args);
	if (node)
	{
		if (((t_var *)node->content)->flag == VAR_LOCAL)
			update_shell_var(node, value, VAR_LOCAL);
		else
			update_shell_var(node, value, VAR_ENV);
	}
	else
		create_shell_var(vars, args, value, VAR_LOCAL);
}

int	exec_local_vars(char **args, t_list **vars)
{
	char	*assign;
	char	*value;
	size_t	i;

	if (!args || !vars)
		return (1);
	value = NULL;
	i = 0;
	while (args[i])
	{
		assign = ft_strchr(args[i], '=');
		*assign = '\0';
		if (ft_strchr(args[i], '+'))
		{
			*(assign - 1) = '\0';
			value = expand_env(*vars, args[i]);
			value = ft_strjoin(value, assign + 1);
		}
		else
			value = ft_strdup(assign + 1);
		_set_shell_var(vars, args[i++], value);
	}
	free(value);
	return (0);
}
