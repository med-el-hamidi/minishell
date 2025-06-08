# include "../../includes/minishell.h"

void	free_2d_array(char **arr)
{
	char	**ptr;

	if (!arr)
		return ;
	ptr = arr;
	while (*ptr)
		free(*ptr++);
	free(arr);
}

void	del_env(void *v)
{
	t_env	*ptr;

	if (!v)
		return ;
	ptr = (t_env *)v;
	free(ptr->key);
	free(ptr->value);
	free(ptr);
}

void	del_token(void *t)
{
	t_token	*ptr;

	if (!t)
		return ;
	ptr = (t_token *)t;
	free(ptr->value);
	free(ptr);
}

void free_ast(t_ast *node)
{
	if (!node)
        return;
    // Free command arguments
	if (node->type == AST_CMD && node->args)
		free_2d_array(node->args);
	// Free filename (for redirection nodes)
	if (node->type == AST_REDIR && node->redir_file)
		free(node->redir_file);
    // Recursively free children
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	free(node);
}
