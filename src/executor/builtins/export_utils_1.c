/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:25:39 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/08 11:26:48 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	update_shell_var(t_list *node, char *value, t_var_type flag)
{
	t_var	*var;

	if (!node)
	{
		errno = ENOMEM;
		perror("malloc export_update_node failed");
		return ;
	}
	var = (t_var *)node->content;
	if (var->value)
		free(var->value);
	if (value)
		var->value = ft_strdup(value);
	else
		var->value = NULL;
	var->flag = flag;
}

void	create_shell_var(t_list **vars, char *key, char *value, t_var_type flag)
{
	t_var	*node;

	node = malloc(sizeof(t_var));
	if (!node)
	{
		errno = ENOMEM;
		perror("malloc export_add_node failed");
		return ;
	}
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->flag = flag;
	ft_lstadd_back(vars, ft_lstnew(node));
}
