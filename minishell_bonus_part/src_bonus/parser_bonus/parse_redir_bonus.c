/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redir_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 19:09:18 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 19:09:20 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

static int	_get_redir_type(int type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_INPUT);
	else if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUTPUT);
	else if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	else if (type == TOKEN_REDIR_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_NONE);
}

static t_ast	*_new_redir_node(t_ast *cmd, t_token *redir_tok)
{
	t_ast	*redir_node;

	if (!redir_tok)
		return (NULL);
	redir_node = new_ast_node(AST_REDIR, NULL);
	if (!redir_node)
		return (NULL);
	redir_node->left = cmd;
	redir_node->redir_type = _get_redir_type(redir_tok->type);
	redir_node->redir_fd = -1;
	redir_node->redir_file = ft_strdup(redir_tok->value);
	if (!redir_node->redir_file)
		return (free(redir_node), NULL);
	return (redir_node);
}

t_ast	*parse_redirection(t_list **tokens, t_ast *command)
{
	t_ast	*redir_node;
	t_token	*redir_tok;

	if (!tokens)
		return (NULL);
	while (*tokens && is_redirection(((t_token *)(*tokens)->content)->type))
	{
		redir_tok = (t_token *)(*tokens)->content;
		redir_node = _new_redir_node(command, redir_tok);
		if (!redir_node)
			return (free_ast(command), NULL);
		command = redir_node;
		advance_token(tokens);
	}
	return (command);
}

t_ast	*parse_leading_redirection(t_list **tokens)
{
	t_ast	*redir_chain;

	redir_chain = NULL;
	while (*tokens && is_redirection(((t_token *)(*tokens)->content)->type))
	{
		redir_chain = parse_redirection(tokens, redir_chain);
		if (!redir_chain)
			return (NULL);
	}
	return (redir_chain);
}
