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
	t_list	*ptr;
	t_list	*prev;
	int		p_open;
	int		p_body;

	if (!tokens)
		return (1);
	if (is_special(((t_token *)tokens->content)->type) || \
		is_special(((t_token *)ft_lstlast(tokens)->content)->type))
		return (print_syntax_error(((t_token *)tokens->content)->value));
	else if (((t_token *)tokens->content)->type == TOKEN_P_CLOSE || \
			((t_token *)ft_lstlast(tokens)->content)->type == TOKEN_P_OPEN)
		return (print_syntax_error(((t_token *)tokens->content)->value));
	prev = NULL;
	ptr = tokens;
	while (ptr)
	{
		if (is_special(((t_token *)ptr->content)->type) && \
			prev && is_special(((t_token *)prev->content)->type))
			return (print_syntax_error(((t_token *)ptr->content)->value));
		else if (is_redirection(((t_token *)tokens->content)->type)
			&& !((t_token *)tokens->content)->value)
			return (SNTX_EXIT_STATUS);
		prev = ptr;
		ptr = ptr->next;
	}
	p_open = 0;
	p_body = 0;
	prev = NULL;
	ptr = tokens;
	while (ptr)
	{
		if (p_open)
		{
			p_open++;
			if (p_open == 2 && ((t_token *)ptr->content)->type != TOKEN_WORD && !is_redirection(((t_token *)ptr->content)->type))
				return (print_syntax_error(((t_token *)ptr->content)->value));
			else if (is_special(((t_token *)ptr->content)->type))
				p_body = 1;
		}
		if (!p_open && ((t_token *)ptr->content)->type == TOKEN_P_OPEN)
		{
			if (prev && !is_special(((t_token *)prev->content)->type))
				return (print_syntax_error(((t_token *)ptr->content)->value));
			p_open = 1;
			p_body = 0;
		}
		else if (((t_token *)ptr->content)->type == TOKEN_P_CLOSE)
		{
			if ((prev  && ((t_token *)prev->content)->type != TOKEN_WORD && !is_redirection(((t_token *)prev->content)->type)) || !p_body)
				return (print_syntax_error(((t_token *)ptr->content)->value));
			p_open = 0;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return (0);
}

static int	get_ast_op(int token_type)
{
	if (token_type == TOKEN_PIPE)
		return (AST_PIPE);
	if (token_type == TOKEN_AND)
		return (AST_AND);
	if (token_type == TOKEN_OR)
		return (AST_OR);
	return (-1);
}

int	get_precedence(int token_type)
{
	if (token_type == TOKEN_OR)
		return (1);
	if (token_type == TOKEN_AND)
		return (2);
	if (token_type == TOKEN_PIPE)
		return (3);
	return (-1);
}

t_ast	*build_ast(t_list **tokens, int min_precedence)
{
	t_ast	*left;
	t_ast	*right;
	t_ast	*node;
	int		op;

	if (!tokens)
		return (NULL);
	left = parse_command(tokens);
	if (!left)
		return (NULL);
	while (*tokens && \
		get_precedence(((t_token *)(*tokens)->content)->type) >= min_precedence)
	{
		op = get_ast_op(((t_token *)(*tokens)->content)->type);
		advance_token(tokens);
		right = build_ast(tokens, min_precedence + 1);
		if (!right)
			return (free_ast(left), NULL);
		node = new_ast_node(op, NULL);
		if (!node)
			return (free_ast(left), free_ast(right), NULL);
		node->left = left;
		node->right = right;
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
	ast = build_ast(&tokens, 0);
	shell->exit_status = syntax_error_ast(ast);
	if (!ast || syntax_error_ast(ast))
	{
		free_ast(ast);
		return (NULL);
	}
	return (ast);
}
