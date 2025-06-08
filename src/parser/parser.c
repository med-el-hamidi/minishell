#include "../../includes/minishell.h"

static int	syntax_error(t_list *tokens)
{
	if (!tokens)
		return (1);
	if (((t_token *)tokens->content)->type == TOKEN_PIPE
		|| ((t_token *)ft_lstlast(tokens)->content)->type == TOKEN_PIPE)
		return (1);
	while (tokens)
	{
		if (is_redirection(((t_token *)tokens->content)->type))
		{
			if (!tokens->next
				|| ((t_token *)tokens->next->content)->type != TOKEN_WORD)
				return (1);
		}
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
		return (!node->redir_file || !node->left || syntax_error_ast(node->left));
	return (0);
}

t_ast	*parser(t_list *tokens)
{
	t_ast	*ast;

	if (!tokens || syntax_error(tokens))
		return (NULL);
	ast = build_ast(&tokens);
	if (!ast || syntax_error_ast(ast))
	{
		free_ast(ast);
		return (NULL);
	}
	return (ast);
}