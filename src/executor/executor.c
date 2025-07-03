#include "../../includes/minishell.h"

int	executor(t_ast *node, t_shell *shell)
{
	if (!node)
		return (1);
	if (node->type == AST_REDIR)
		return (exec_redirection(node, shell));
	else if (node->type == AST_PIPE)
		return (exec_pipe(node, shell));
	else if (node->type == AST_CMD && node->args)
	{
		if (is_builtin(node->args[0]))
			return (exec_builtins(node, shell));
		else
			return (exec_external(node, shell));
	}
	return (1);
}
