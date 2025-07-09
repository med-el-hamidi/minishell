/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:14:50 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/09 01:27:10 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*join_3(const char *s1, char *s2, const char *s3)
{
	char	*res;
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

static int	_is_valid_local_var(const char *cmd)
{
	int	i;

	i = 0;
	if (!cmd || (!ft_isalpha(cmd[i]) && cmd[i] != '_'))
		return (0);
	else
		i++;
	while (cmd[i])
	{
		if (cmd[i] == '=' || ((cmd[i] == '+' && cmd[i + 1] == '=')))
			return (1);
		if (!ft_isalnum(cmd[i]) && cmd[i] != '_')
			return (0);
		i++;
	}
	return (0);
}

static void	_set_new_args(char **args, size_t *i, size_t *j)
{
	size_t	count;

	count = ft_argv_count(args);
	*j = 0;
	while (*j + *i < count)
	{
		free(args[*j]);
		args[*j] = ft_strdup(args[*j + *i]);
		j++;
	}
	while (*j < count)
	{
		free(args[*j]);
		args[(*j)++] = NULL;
	}
}

int	_is_local_vars(char **args)
{
	size_t	i;
	size_t	j;

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
				_set_new_args(args, &i, &j);
			return (0);
		}
	}
	return (1);
}
