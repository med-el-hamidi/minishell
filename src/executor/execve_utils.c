/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 02:01:55 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/13 02:04:01 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	_envp_helper(t_var *env, char **envp, int *i)
{
	if (!env || !env->key)
		return (1);
	envp[*i] = join_3(env->key, "=", env->value);
	if (!envp[(*i)++])
		return (free_2d_array(envp), 1);
	return (0);
}

char	**env_list_to_envp(t_list *vars)
{
	t_list	*ptr;
	t_var	*env;
	int		i;
	char	**envp;

	i = 0;
	envp = malloc(sizeof(char *) * (ft_lstsize(vars) + 1));
	if (!envp)
		return (NULL);
	ptr = vars;
	while (ptr)
	{
		env = (t_var *)ptr->content;
		if (env->flag == VAR_ENV && _envp_helper(env, envp, &i))
			return (NULL);
		ptr = ptr->next;
	}
	envp[i] = NULL;
	return (envp);
}
