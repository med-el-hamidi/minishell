/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_0.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:27:46 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/08 11:51:38 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	_is_exist(char **keys, char *key)
{
	int	i;

	if (!keys)
		return (0);
	i = 0;
	while (keys[i])
	{
		if (!ft_strcmp(keys[i], key))
			return (1);
		i++;
	}
	return (0);
}

static t_var	*find_max(t_list *vars)
{
	t_list	*ptr;
	t_var	*var;
	t_var	*max_var;

	if (!vars)
		return (NULL);
	max_var = (t_var *)vars->content;
	ptr = vars;
	while (ptr)
	{
		var = (t_var *)ptr->content;
		if (ft_strcmp(max_var->key, var->key) < 0)
			max_var = var;
		ptr = ptr->next;
	}
	return (max_var);
}

static void	find_min(t_list *vars, t_var **min_var, char **min_keys)
{
	t_list	*ptr;
	t_var	*var;
	t_var	*tmp;

	tmp = find_max(vars);
	ptr = vars;
	while (ptr)
	{
		var = (t_var *)ptr->content;
		if (!_is_exist(min_keys, var->key) \
			&& ft_strcmp(tmp->key, var->key) > 0)
			tmp = var;
		ptr = ptr->next;
	}
	*min_var = tmp;
}

static void	put_out_exported(t_list *vars, t_var *min_var)
{
	if (min_var->value)
		printf("declare -x %s=\"%s\"\n", min_var->key, \
			expand_env(vars, min_var->key));
	else
		printf("declare -x %s\n", min_var->key);
}

int	_export(t_list *vars)
{
	t_var	*min_var;
	char	**printed;
	int		len;
	int		i;

	len = ft_lstsize(vars);
	printed = malloc(sizeof(char *) * (len + 1));
	if (!printed)
		return ((errno = ENOMEM), perror("malloc export_utils failed"), 1);
	printed[0] = NULL;
	i = 0;
	while (len--)
	{
		find_min(vars, &min_var, printed);
		if (min_var && !_is_exist(printed, min_var->key) \
			&& (min_var->flag == VAR_ENV || min_var->flag == VAR_EXPORTED))
		{
			put_out_exported(vars, min_var);
			printed[i++] = ft_strdup(min_var->key);
			if (!printed[i - 1])
				return (free_2d_array(printed), (errno = ENOMEM), 1);
			printed[i] = NULL;
		}
	}
	return (free_2d_array(printed), 0);
}
