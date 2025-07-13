/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:40:40 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/13 22:47:59 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	_handl_empty_f(char **envp, char *path)
{
	char	*sh_argv[3];

	if (errno == ENOEXEC)
	{
		sh_argv[0] = "/bin/sh";
		sh_argv[1] = path;
		sh_argv[2] = NULL;
		execve("/bin/sh", sh_argv, envp);
	}
}

static void	_child_execve(t_ast *node, char *path, char **envp)
{
	signal(SIGQUIT, SIG_DFL);
	if (execve(path, node->args, envp) == -1)
		_handl_empty_f(envp, path);
	ft_putstr_fd("minishell: execve ", STDERR_FILENO);
	perror(node->args[0]);
	exit (126);
}

static void	_handl_child_exit_st(t_shell *shell, int status)
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
	if (node->args[0] && !*node->args[0])
		return (ft_putendl_fd("Command '' not found", 2), 127);
	else if (is_directory(node->args[0]))
		return (execv_print_error(node->args[0], 4), 126);
	path = get_cmd_path(node->args[0], shell);
	if (!path)
		return (127);
	envp = env_list_to_envp(shell->vars);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
		_child_execve(node, path, envp);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	_handl_child_exit_st(shell, status);
	return (free(path), free_2d_array(envp), shell->exit_status);
}
