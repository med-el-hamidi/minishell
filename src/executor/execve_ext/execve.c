/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:40:40 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/15 23:52:46 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	is_empty_or_dir_cmd(t_shell *shell, char *arg)
{
	if (arg && !*arg)
	{
		shell->exit_status = 127;
		return (ft_putendl_fd("Command '' not found", 2), 1);
	}
	else if (arg[0] == '/' || !strcmp(arg, "./") || !strcmp(arg, "../"))
	{
		if (is_directory(arg))
		{
			shell->exit_status = 126;
			return (execv_print_error(arg, 4), 1);
		}
	}
	return (0);
}

static void	_execve(t_ast *node, char *path, char **envp)
{
	char	*sh_argv[3];

	signal(SIGQUIT, SIG_DFL);
	if (execve(path, node->args, envp) == -1)
	{
		if (errno == ENOEXEC)
		{
			sh_argv[0] = "/bin/sh";
			sh_argv[1] = path;
			sh_argv[2] = NULL;
			execve("/bin/sh", sh_argv, envp);
		}
	}
	ft_putstr_fd("minishell: execve ", STDERR_FILENO);
	perror(node->args[0]);
	exit (126);
}

static void	_set_exit_status(t_shell *shell, int status)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

int	exec_external(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**envp;

	if (!node || !node->args)
		return (0);
	if (is_empty_or_dir_cmd(shell, node->args[0]))
		return (shell->exit_status);
	path = get_cmd_path(node->args[0], shell);
	if (!path && is_directory(node->args[0]))
		return (free(path), execv_print_error(node->args[0], 4), 126);
	else if (!path)
		return (free(path), 127);
	envp = env_list_to_envp(shell->vars);
	pid = fork();
	if (pid == -1)
		return (free(path), perror("fork"), 1);
	else if (pid == 0)
		_execve(node, path, envp);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	_set_exit_status(shell, status);
	return (free(path), free_2d_array(envp), shell->exit_status);
}
