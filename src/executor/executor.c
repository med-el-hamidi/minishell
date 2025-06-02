#include "../../includes/minishell.h"

/*
***TO DO***
exec_external
exec_pipe
exec_redirection
*/
int	executor(t_ast *node, t_shell *shell)
{
	if (!node)
		return (1);
	if (node->type == AST_CMD)
	{
		if (is_builtin(node->args[0]))
			return (exec_builtins(node, shell));
		else
			return (0);
			// return (exec_external(node, shell));
	}
	else if (node->type == AST_PIPE)
		return (0);// return (exec_pipe(node, shell));
	else if (node->type == AST_REDIR)
		return (0);//return (exec_redirection(node, shell));
	return (1);
}
