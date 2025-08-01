/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:12:34 by obensarj          #+#    #+#             */
/*   Updated: 2025/08/01 14:45:24 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "export"));
}
static int	exec_builtins(t_ast *node, t_shell *shell)
{
	if (!ft_strcmp(node->args[0], "echo"))
		return (builtin_echo(node->args));
	if (!ft_strcmp(node->args[0], "cd"))
		return (builtin_cd(node->args, shell));
	if (!ft_strcmp(node->args[0], "exit"))
		builtin_exit(node, shell);
	if (!ft_strcmp(node->args[0], "unset"))
		return (builtin_unset(node->args, &shell->vars));
	if (!ft_strcmp(node->args[0], "env"))
		return (builtin_env(shell->vars));
	if (!ft_strcmp(node->args[0], "pwd"))
		return (builtin_pwd(shell->vars));
	if (!ft_strcmp(node->args[0], "export"))
		return (builtin_export(node->args, &shell->vars));
	return (1);
}

int	executor(t_ast *node, t_shell *shell)
{
	if (!node)
		return (shell->exit_status);
	if (node->type == AST_REDIR)
		return (exec_redirection(node, shell));
	else if (node->type == AST_PIPE)
		return (exec_pipe(node, shell));
	else if (node->type == AST_CMD && node->args)
	{
		if (!node->args[0])
			return (0);
		if (_is_local_vars(node))
			return (exec_local_vars(node->args, &shell->vars));
		else if (is_builtin(node->args[0]))
			return (exec_builtins(node, shell));
		else
			return (exec_external(node, shell));
	}
	return (1);
}
