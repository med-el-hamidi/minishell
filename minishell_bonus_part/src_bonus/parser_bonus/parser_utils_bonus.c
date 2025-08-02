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

int	is_control_operator(t_token_type type)
{
	return (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR);
}

int	is_special(int type)
{
	return (is_control_operator(type)
		|| type == TOKEN_P_OPEN || type == TOKEN_P_CLOSE);
}
