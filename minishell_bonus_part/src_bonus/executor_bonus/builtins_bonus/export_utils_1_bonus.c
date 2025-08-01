/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_1_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/08 11:25:39 by obensarj          #+#    #+#             */
/*   Updated: 2025/08/01 21:14:35 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes_bonus/minishell_bonus.h"

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

static void	_get_lastarg(char **last_arg, t_token *token, int *paren_level,
	int *f)
{
	if (token->type == TOKEN_P_OPEN)
		(*paren_level)++;
	else if (token->type == TOKEN_P_CLOSE)
		(*paren_level)--;
	else if (!*paren_level && token->type == TOKEN_PIPE)
		*f = 1;
	else if (!*paren_level
		&& (token->type == TOKEN_AND || token->type == TOKEN_OR))
		*f = 0;
	else if (!*f && !*paren_level && token->type == TOKEN_WORD && token->value)
		*last_arg = token->value;
}

void	update_lastarg_var(t_list *tokens, t_list *vars)
{
	char	*last_arg;
	int		paren_level;
	int		f;

	last_arg = NULL;
	paren_level = 0;
	f = 0;
	while (tokens)
	{
		_get_lastarg(&last_arg, (t_token *)tokens->content, &paren_level, &f);
		tokens = tokens->next;
	}
	if (!f && last_arg)
		update_shell_var(find_shell_var(vars, "_"), last_arg, VAR_ENV);
	else if (f)
		update_shell_var(find_shell_var(vars, "_"), "", VAR_ENV);
}
