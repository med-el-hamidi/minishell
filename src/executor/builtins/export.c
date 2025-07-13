/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:30:10 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/11 23:54:50 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	is_valid_identifier(const char *arg)
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

static void	_add_or_update_var(t_list **vars, char *arg, char **alloc_value)
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
}

static void	_handle_arg(t_list **vars, char *arg)
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
	{
		_add_or_update_var(vars, arg, &alloc_value);
	}
	if (alloc_value)
		free(alloc_value);
}

static int	_export_var(char **arg, t_list **vars)
{
	int		i;
	char	**invalid_args;
	int		invalid_count;
	int		arg_counter;

	arg_counter = count_2d_array(arg);
	invalid_args = malloc(sizeof(char *) * arg_counter);
	if (!invalid_args)
		return (perror("malloc export failed"), 1);
	invalid_count = 0;
	i = 0;
	while (arg[++i])
	{
		if (!is_valid_identifier(arg[i]))
			invalid_args[invalid_count++] = arg[i];
		else
			_handle_arg(vars, arg[i]);
	}
	i = -1;
	while (++i < invalid_count)
		export_print_error(invalid_args[i]);
	free(invalid_args);
	return (invalid_count > 0);
}

int	builtin_export(char **argv, t_list **vars)
{
	if (!argv || !vars)
		return (1);
	if (!argv[1])
		return (_export(*vars));
	if (_export_var(argv, vars))
		return (1);
	return (0);
}
