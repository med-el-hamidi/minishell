/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:25:39 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/13 16:54:40 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	update_shell_var(t_list *node, char *value, t_var_type flag)
{
	t_var	*var;

	if (!node)
		return ;
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
		perror("failed to malloc create_shell_var");
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

void	_add_or_update_path_cd(t_list **vars, char *name, char *value)
{
	t_list	*node;

	node = find_shell_var(*vars, name);
	if (node)
		update_shell_var(node, value, VAR_ENV);
	else
		create_shell_var(vars, name, value, VAR_ENV);
}

void	update_lastarg_var(t_list *tokens, t_list *vars)
{
	t_token	*token;
	char	*last_arg;
	int		f;

	last_arg = NULL;
	f = 0;
	while (tokens)
	{
		token = (t_token *)tokens->content;
		if (token->type == TOKEN_PIPE)
			f = 1;
		else if (!f && token->type == TOKEN_WORD && token->value)
			last_arg = token->value;
		tokens = tokens->next;
	}
	if (!f && last_arg)
		update_shell_var(find_shell_var(vars, "_"), last_arg, VAR_ENV);
	else
		update_shell_var(find_shell_var(vars, "_"), "", VAR_ENV);
}
