/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:08:38 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:08:39 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

static int	syntax_error(t_list *tokens)
{
	if (!tokens)
		return (1);
	if (((t_token *)tokens->content)->type == TOKEN_PIPE
		|| ((t_token *)ft_lstlast(tokens)->content)->type == TOKEN_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token '|'", \
																STDERR_FILENO);
		return (2);
	}
	while (tokens)
	{
		if (is_redirection(((t_token *)tokens->content)->type)
			&& !((t_token *)tokens->content)->value)
			return (2);
		tokens = tokens->next;
	}
	return (0);
}

static t_ast	*build_ast(t_list **tokens)
{
	t_ast	*left;
	t_ast	*node;

	left = parse_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((t_token *)(*tokens)->content)->type == TOKEN_PIPE)
	{
		advance_token(tokens);
		node = new_ast_node(AST_PIPE, NULL);
		if (!node)
		{
			free_ast(left);
			return (NULL);
		}
		node->left = left;
		node->right = parse_command(tokens);
		if (!node->right)
		{
			free_ast(node);
			return (NULL);
		}
		left = node;
	}
	return (left);
}

static int	syntax_error_ast(t_ast *node)
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

t_ast	*parser(t_list *tokens, t_shell *shell)
{
	t_ast	*ast;

	g_exit_status = 0;
	shell->exit_status = syntax_error(tokens);
	if (!tokens || shell->exit_status)
		return (NULL);
	ast = build_ast(&tokens);
	shell->exit_status = syntax_error_ast(ast);
	if (!ast || syntax_error_ast(ast))
	{
		free_ast(ast);
		return (NULL);
	}
	return (ast);
}
