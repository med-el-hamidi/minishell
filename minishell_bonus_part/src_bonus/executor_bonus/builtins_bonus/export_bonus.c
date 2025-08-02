#include "../../../includes_bonus/minishell_bonus.h"

int	is_valid_identifier(const char *arg)
{
	int	i;

	i = 0;
	if (!arg || (!ft_isalpha(arg[i]) && arg[i] != '_'))
		return (0);
	else
		i++;
	while (arg[i])
	{
		if (arg[i] == '=' || (arg[i] == '+' && arg[i + 1] == '='))
			return (1);
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

static char	*_add_or_update_var(t_list **vars, char *arg, char **alloc_value)
{
	t_list	*node;
	char	*assign;
	char	*value;

	assign = ft_strchr(arg, '=');
	*assign = '\0';
	if (ft_strchr(arg, '+'))
	{
		*(assign - 1) = '\0';
		value = expand_env(*vars, arg);
		if (value)
		{
			*alloc_value = ft_strjoin(value, assign + 1);
			value = *alloc_value;
		}
		else
			value = assign + 1;
	}
	else
		value = assign + 1;
	node = find_shell_var(*vars, arg);
	if (node)
		update_shell_var(node, value, VAR_ENV);
	else
		create_shell_var(vars, arg, value, VAR_ENV);
	return (arg);
}

static void	_handle_arg(t_list **vars, char *arg, char **last_arg)
{
	t_list	*node;
	char	*assign;
	char	*alloc_value;

	alloc_value = NULL;
	assign = ft_strchr(arg, '=');
	if (!assign)
	{
		node = find_shell_var(*vars, arg);
		if (node)
		{
			if (((t_var *)node->content)->flag == VAR_LOCAL)
				((t_var *)node->content)->flag = VAR_ENV;
		}
		else
			create_shell_var(vars, arg, NULL, VAR_EXPORTED);
	}
	else
		*last_arg = _add_or_update_var(vars, arg, &alloc_value);
	if (alloc_value)
		free(alloc_value);
}

static int	_export_var(char **args, t_list **vars, char **last_arg)
{
	int		i;
	char	**invalid_args;
	int		invalid_count;
	int		arg_counter;

	arg_counter = count_2d_array(args);
	invalid_args = malloc(sizeof(char *) * arg_counter);
	if (!invalid_args)
		return (perror("malloc export failed"), 1);
	invalid_count = 0;
	i = 0;
	while (args[++i])
	{
		if (!is_valid_identifier(args[i]))
		{
			invalid_args[invalid_count++] = args[i];
			if (!args[i + 1])
				*last_arg = NULL;
		}
		else
			_handle_arg(vars, args[i], last_arg);
	}
	i = -1;
	while (++i < invalid_count)
		export_print_error(invalid_args[i]);
	free(invalid_args);
	return (invalid_count > 0);
}

int	builtin_export(char **args, t_list **vars)
{
	char	*last_arg;

	if (!args || !vars)
		return (1);
	if (!args[1])
		return (_export(*vars));
	last_arg = NULL;
	if (_export_var(args, vars, &last_arg))
		return (1);
	if (last_arg)
		update_shell_var(find_shell_var(*vars, "_"), last_arg, VAR_ENV);
	return (0);
}
