/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:12:34 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/26 18:26:07 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

static int	_check_bonus_part(t_ast *node, t_shell *shell);

int	executor(t_ast *node, t_shell *shell)
{
	if (!node)
		return (shell->exit_status);
	if (node->type == AST_REDIR)
		return (exec_redirection(node, shell));
	else if (node->type == AST_PIPE)
		return (exec_pipe(node, shell));
	else if (node->type == AST_AND || node->type == AST_OR)
		return (_check_bonus_part(node, shell));
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

static int	_check_bonus_part(t_ast *node, t_shell *shell)
{
	int	status;

	if (node->type == AST_AND)
	{
		status = executor(node->left, shell);
		if (!status)
			status = executor(node->right, shell);
	}
	else if (node->type == AST_OR)
	{
		status = executor(node->left, shell);
		if (status != 0)
			status = executor(node->right, shell);
	}
	else
		status = 1;
	shell->exit_status = status;
	return (status);
}
