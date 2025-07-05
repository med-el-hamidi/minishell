#include "../../includes/minishell.h"

int	is_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "env"));
}

int	exec_builtins(t_ast *node, t_shell *shell)
{
	if (!ft_strcmp(node->args[0], "echo"))
		return (builtin_echo(node->args));
	if (!ft_strcmp(node->args[0], "cd"))
		return (builtin_cd(node->args, shell));
	if (!ft_strcmp(node->args[0], "exit"))
		return (shell->exit_status/*builtin_exit(node->args)*/);
	if (!ft_strcmp(node->args[0], "export"))
		return (builtin_export(node->args, &shell->vars));
	if (!ft_strcmp(node->args[0], "unset"))
		return (builtin_unset(node->args, &shell->vars));
	if (!ft_strcmp(node->args[0], "env"))
		return (builtin_env(shell->vars));
	if (!ft_strcmp(node->args[0], "pwd"))
		return (builtin_pwd());
	return (1);
}
