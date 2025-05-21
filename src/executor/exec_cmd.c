#include "../../includes/minishell.h"

int is_builtin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", 4) ||
			!ft_strncmp(cmd, "cd", 2) ||
			!ft_strncmp(cmd, "exit", 4) ||
			!ft_strncmp(cmd, "export", 6) ||
			!ft_strncmp(cmd, "unset", 5) ||
			!ft_strncmp(cmd, "env", 3));
}

int exec_builtins(t_ast *node, t_shell *shell)
{
	if(!ft_strncmp(node->args[0], "echo", 4))
		return (builtin_echo(node->args));
	if(!ft_strncmp(node->args[0], "cd", 2))
		return (builtin_cd(node->args));
	// +++ REST OF BUILDINS here...
	return (1);
}
