#include "../../includes/minishell.h"

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
