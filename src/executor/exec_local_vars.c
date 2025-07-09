/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_local_vars.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:07:46 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/09 00:02:15 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	exec_local_vars(char **args, t_list **vars)
{
	char	*assign;
	char	*value;
	char	*alloc_value;
	t_list	*node;
	size_t	i;

	alloc_value = NULL;
	i = 0;
	while (args[i])
	{
		assign = ft_strchr(args[i], '=');
		*assign = '\0';
		if (ft_strchr(args[i], '+'))
		{
			*(assign - 1) = '\0';
			value = expand_env(*vars, args[i]);
			if (value)
			{
				alloc_value = ft_strjoin(value,  assign + 1);
				value = alloc_value;
			}
			else
				value = assign + 1;
		}
		else
			value = assign + 1;
		node = find_shell_var(*vars, args[i]);
		if (node)
		{
			if (((t_var *)node->content)->flag == VAR_LOCAL)
				update_shell_var(node, value, VAR_LOCAL);
			else
				update_shell_var(node, value, VAR_ENV);
		}
		else
			create_shell_var(vars, args[i], value, VAR_LOCAL);
		i++;
		if (alloc_value)
			free(alloc_value);
	}
	return (0);
}
