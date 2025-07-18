/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:09:43 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:09:45 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

t_ast	*new_ast_node(int type, char **args)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = args;
	node->left = NULL;
	node->right = NULL;
	node->redir_file = NULL;
	node->redir_type = REDIR_NONE;
	node->redir_fd = -1;
	return (node);
}

void	advance_token(t_list **tokens)
{
	if (*tokens)
		*tokens = (*tokens)->next;
}

int	is_redirection(int type)
{
	return (type == TOKEN_REDIR_OUT || type == TOKEN_REDIR_APPEND
		|| type == TOKEN_REDIR_IN || type == TOKEN_REDIR_HEREDOC
		|| type == TOKEN_AMB_REDIR);
}
