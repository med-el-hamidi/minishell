#include "../../../includes/minishell.h"

static int	handle_no_path_env(t_shell *shell, char *arg)
{
	char	*env;

	env = expand_env(shell->vars, "PATH");
	if (!env)
	{
		if (is_directory(arg))
		{
			shell->exit_status = 126;
			execve_print_error(arg, 4);
		}
		else
		{
			shell->exit_status = 127;
			execve_print_error(arg, 2);
		}
		return (1);
	}
	return (0);
}

static void	_execve(t_ast *node, char *path, char **envp)
{
	signal(SIGQUIT, SIG_DFL);
	if (execve(path, node->args, envp) == -1)
	{
		if (errno == ENOEXEC)
			exit(0);
	}
	ft_putstr_fd("minishell: execve ", STDERR_FILENO);
	perror(path);
	exit (126);
}

static void	_set_exit_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

static int	_execve_print_error(t_shell *shell, char *arg)
{
	if (is_directory(arg) && (arg[0] == '/' || \
		(ft_strlen(arg) > 0 && arg[ft_strlen(arg) - 1] == '/') || \
		!strncmp(arg, "./", 2) || !strncmp(arg, "../", 3)))
	{
		shell->exit_status = 126;
		execve_print_error(arg, 4);
	}
	else if (ft_strchr(arg, '/'))
	{
		shell->exit_status = 127;
		execve_print_error(arg, 2);
	}
	else
	{
		shell->exit_status = 127;
		execve_print_error(arg, 1);
	}
	return (shell->exit_status);
}

int	exec_external(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**envp;

	if (!node || !node->args || !node->args[0])
		return (0);
	if (handle_no_path_env(shell, node->args[0]))
		return (shell->exit_status);
	path = get_cmd_path(node->args[0], shell);
	if (!path)
		return (_execve_print_error(shell, node->args[0]));
	envp = env_list_to_envp(shell->vars);
	pid = fork();
	if (pid == -1)
		return (free(path), perror("fork"), 1);
	else if (pid == 0)
		_execve(node, path, envp);
	signal(SIGINT, SIG_IGN);
	(waitpid(pid, &status, 0), setup_signals());
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL)
		(tcsetattr(STDIN_FILENO, TCSANOW, &shell->new_termios), \
		ft_putchar_fd('\n', 1));
	_set_exit_status(shell, status);
	return (free(path), free_2d_array(envp), shell->exit_status);
}
