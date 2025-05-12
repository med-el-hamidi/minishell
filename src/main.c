#include "../includes/minishell.h"

/*
*
* Main Function
* envp:	Environment pointer (array of environment variable strings), its format: "KEY=value" ex. PATH=/usr/bin:/bin
*
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	shell_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}

void init_shell(t_shell *shell, char **envp)
{
    (void)shell;
    (void)envp;
}

void shell_loop(t_shell *shell)
{
    (void)shell;
}

void cleanup_shell(t_shell *shell)
{
    (void)shell;
}
