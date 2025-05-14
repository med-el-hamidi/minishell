#include "../includes/minishell.h"

/*
*
* Main Function
* envp:	Environment pointer (array of environment variable strings), its format: "KEY=value" ex. PATH=/usr/bin:/bin
*
*/
int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;
	init_shell(&shell, envp);
	shell_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}

/*
* init_shell: Initialize minishell
*
* 1. Initialize environment variables (create copy)
* 2. Set default exit status & is_interactive
* 3. Backup standard file descriptors
* 4. Initialize signal handlers
* 5. Initialize history
*/
void init_shell(t_shell *shell, char **envp)
{
	shell->env = copy_env(envp);
	if (!shell->env)
		exit_error("Failed to initialize the environment\n", 1);
	shell->exit_status = 0;
	shell->is_interactive = isatty(STDIN_FILENO);
	shell->stdin_fd = dup(STDIN_FILENO);
	shell->stdout_fd = dup(STDOUT_FILENO);
	if (shell->stdin_fd == -1 || shell->stdout_fd == -1)
		exit_error("Failed to backup std file descriptors", 0);
	setup_signals();
	//if (shell->is_interactive)
	//	init_history();
}

void shell_loop(t_shell *shell)
{
    (void)shell;
	while (1)
		pause();
}

void cleanup_shell(t_shell *shell)
{
	free_2d_array(shell->env);
}
