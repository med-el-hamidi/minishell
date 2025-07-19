/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:12:34 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/19 11:03:19 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

int	executor(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (!node)
		return (shell->exit_status);
	if (node->type == AST_REDIR)
		return (exec_redirection(node, shell));
	else if (node->type == AST_PIPE)
		return (exec_pipe(node, shell));
	else if (node->type == AST_AND)
	{
		status = executor(node->left, shell);
		if (!status)
			status = executor(node->right, shell);
		return (status);
	}
	else if (node->type == AST_OR)
	{
		status = executor(node->left, shell);
		if (status != 0)
			status = executor(node->right, shell);
		return (status);
	}
	else if (node->type == AST_SUBSHELL && node->left)
	{
		pid = fork();
		if (pid == -1)
			return (perror("subshell fork"), 1);
		if (!pid)
			exit(executor(node->left, shell));
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		return (1);
	}
	else if (node->type == AST_CMD && node->args)
	{
		if (_is_local_vars(node))
			return (exec_local_vars(node->args, &shell->vars));
		else if (is_builtin(node->args[0]))
			return (exec_builtins(node, shell));
		else
			return (exec_external(node, shell));
	}
	return (1);
}
