/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 19:09:18 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 19:09:20 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

static t_ast	*_new_redir_node(t_ast *command, char *file, int type)
{
	t_ast	*redir_node;

	redir_node = new_ast_node(AST_REDIR, NULL);
	if (!redir_node)
		return (NULL);
	redir_node->left = command;
	redir_node->redir_file = ft_strdup(file);
	if (!redir_node->redir_file)
	{
		free(redir_node);
		return (NULL);
	}
	redir_node->redir_fd = -1;
	redir_node->redir_type = _get_redir_type(type);
	return (redir_node);
}

t_ast	*parse_redirection(t_list **tokens, t_ast *command)
{
	t_ast	*redir_node;
	t_token	*redir_tok;
	t_token	*file_tok;

	while (*tokens && is_redirection(((t_token *)(*tokens)->content)->type))
	{
		redir_tok = (t_token *)(*tokens)->content;
		advance_token(tokens);
		if (!*tokens || ((t_token *)(*tokens)->content)->type != TOKEN_WORD)
			return (NULL);
		file_tok = (t_token *)(*tokens)->content;
		redir_node = _new_redir_node(command, file_tok->value, redir_tok->type);
		if (!redir_node)
			return (NULL);
		command = redir_node;
		if (command->redir_type == REDIR_HEREDOC)
			set_herdoc_tmp_file(command);
		advance_token(tokens);
	}
	return (command);
}
