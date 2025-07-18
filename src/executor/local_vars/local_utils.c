/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:14:50 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/13 19:24:11 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

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

static void	_set_new_args(t_ast	*node, size_t *i)
{
	char	**new_args;
	size_t	j;
	size_t	count;

	count = count_2d_array(node->args);
	j = 0;
	while (j + *i < count)
		j++;
	new_args = malloc((j + 1) * sizeof(char *));
	if (!new_args)
		return ;
	j = 0;
	while (j + *i < count)
	{
		new_args[j] = ft_strdup(node->args[j + *i]);
		j++;
	}
	new_args[j] = NULL;
	free_2d_array(node->args);
	node->args = new_args;
}

int	_is_local_vars(t_ast *node)
{
	size_t	i;

	if (!node || !node->args)
		return (0);
	i = 0;
	while (node->args[i])
	{
		if (_is_valid_local_var(node->args[i]))
			i++;
		else
		{
			if (i)
				_set_new_args(node, &i);
			return (0);
		}
	}
	return (1);
}
