#include "../../includes/minishell.h"

static void handle_sigint(int sig)
{
	(void)sig;
	ft_putchar_fd('\n', STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	//g_exit_status = 130; // Set exit status (128 + SIGINT=2)
}

void	setup_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &sa, NULL) == -1)
		exit_error("SIGQUIT (Ctrl+\\) handler failed", 1);
	sa.sa_handler = handle_sigint;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		exit_error("SIGINT (Ctrl+C) handler failed", 1);
	// Handle EOF (Ctrl+D)
}
