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

static int	get_ast_op(int token_type)
{
	if (token_type == TOKEN_AND)
		return (AST_AND);
	if (token_type == TOKEN_OR)
		return (AST_OR);
	return (-1);
}

static t_ast	*build_ast(t_list **tokens)
{
	t_ast	*left;
	t_ast	*node;

	if (!tokens)
		return (NULL);
	left = parse_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && ((t_token *)(*tokens)->content)->type == TOKEN_PIPE)
	{
		advance_token(tokens);
		node = new_ast_node(AST_PIPE, NULL);
		if (!node)
			return (free_ast(left), NULL);
		node->left = left;
		node->right = parse_command(tokens);
		if (!node->right)
			return (free_ast(node), NULL);
		left = node;
	}
	return (left);
}

static t_ast	*build_ast_wraper(t_list **tokens)
{
	t_ast	*left;
	t_ast	*node;

	if (!tokens || !*tokens)
		return (NULL);
	left = build_ast(tokens);
	if (!left)
		return (NULL);
	while (*tokens && (((t_token *)(*tokens)->content)->type == TOKEN_AND
		|| ((t_token *)(*tokens)->content)->type == TOKEN_OR))
	{
		node = new_ast_node(get_ast_op(((t_token *)(*tokens)->content)->type),
				NULL);
		if (!node)
			return (free_ast(left), NULL);
		advance_token(tokens);
		node->left = left;
		node->right = build_ast(tokens);
		if (!node->right)
			return (free_ast(node), NULL);
		left = node;
	}
	return (left);
}

t_ast	*parser(t_list *tokens, t_shell *shell)
{
	t_ast	*ast;

	g_exit_status = 0;
	shell->exit_status = syntax_error(tokens);
	if (!tokens || shell->exit_status)
		return (NULL);
	ast = build_ast_wraper(&tokens);
	shell->exit_status = syntax_error_ast(ast);
	if (!ast || syntax_error_ast(ast))
	{
		free_ast(ast);
		return (NULL);
	}
	return (ast);
}
