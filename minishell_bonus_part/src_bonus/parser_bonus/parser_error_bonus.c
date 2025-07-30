/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:09:38 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:09:39 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

static int	is_control_operator(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR);
}

int	syntax_error(t_list *tokens)
{
	t_list	*curr;
	t_list	*prev;

	if (!tokens)
		return (1);
	if (is_control_operator(((t_token *)tokens->content)->type) || \
		is_control_operator(((t_token *)ft_lstlast(tokens)->content)->type))
		return (print_syntax_error(((t_token *)tokens->content)->value));
	if (((t_token *)tokens->content)->type == TOKEN_P_CLOSE || \
		((t_token *)ft_lstlast(tokens)->content)->type == TOKEN_P_OPEN)
		return (print_syntax_error(((t_token *)tokens->content)->value));
	prev = NULL;
	curr = tokens;
	while (curr)
	{
		if (is_control_operator(((t_token *)curr->content)->type) && \
			prev && is_control_operator(((t_token *)prev->content)->type))
			return (print_syntax_error(((t_token *)curr->content)->value));
		else if (is_redirection(((t_token *)curr->content)->type)
			&& !((t_token *)curr->content)->value)
			return (SNTX_EXIT_STATUS);
		prev = curr;
		curr = curr->next;
	}
	return (0);
}

int	syntax_error_ast(t_ast *node)
{
	if (!node)
		return (1);
	if (node->type == AST_CMD)
		return (!node->args || !node->args[0]);
	else if (node->type == AST_PIPE)
		return (!node->left || !node->right
			|| syntax_error_ast(node->left) || syntax_error_ast(node->right));
	else if (node->type == AST_REDIR)
		return (!node->redir_file);
	return (0);
}
