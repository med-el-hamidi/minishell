/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:14:50 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/07 17:18:23 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	_is_valid_local_var(const char *cmd)
{
	int	i;

	i = 0;
	if (!cmd || (!ft_isalpha(cmd[i]) && cmd[i] != '_'))
		return (0);
	else
		i++;
	while (cmd[i] && cmd[i] != '=')
	{
		if (!ft_isalnum(cmd[i]) && cmd[i] != '_')
			return (0);
		i++;
	}
	if (cmd[i] == '=')
		return (1);
	return (0);
}

int	_is_local_vars(char **args)
{
	size_t	i;
	size_t	j;
	size_t	count;

	if (!args)
		return (0);
	i = 0;
	while (args[i])
	{
		if (_is_valid_local_var(args[i]))
			i++;
		else
		{
			if (i)
			{
				count = ft_argv_count(args);
				j = 0;
				while (j + i < count)
				{
					free(args[j]);
					args[j] = ft_strdup(args[j + i]);
					j++;
				}
				while (j < count)
				{
					free(args[j]);
					args[j++] = NULL;
				}
			}
			return (0);
		}
	}
	return (1);
}
