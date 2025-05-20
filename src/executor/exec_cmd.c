#include "../../includes/minishell.h"

int is_buildin(char *cmd)
{
	return (!ft_strncmp(cmd, "echo", 5) ||
			!ft_strncmp(cmd, "cd", 3) ||
			!ft_strncmp(cmd, "exit", 5) ||
			!ft_strncmp(cmd, "export", 7) ||
			!ft_strncmp(cmd, "unset", 6) ||
			!ft_strncmp(cmd, "env", 4));
}

int exec_buildin(t_ast *node, t_shell *shell)
{
	if(!ft_strncmp(node->args[0], "echo", 5))
		return (buildin_echo(node->args));
	// +++ REST OF BUILDINS here...
	return (1);
}
