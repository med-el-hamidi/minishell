/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:41:10 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/07 22:41:54 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	error_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	exec_pipe(t_ast *node, t_shell *sh)
{
	int		pipefd[2];
	pid_t	child_pid[2];
	int		status[2];
	int		exit_status;

	if (!node || !node->left || !node->right)
		return (EXIT_FAILURE);
	if (pipe(pipefd) == -1)
		return (perror("pipe: "), 1);
	child_pid[0] = fork();
	if (child_pid[0] == -1)
		error_exit("fork");
	if (child_pid[0] == 0)
	{
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			error_exit("dup2");
		close(pipefd[0]);
		close(pipefd[1]);
		exit(executor(node->left, sh));
	}
	child_pid[1] = fork();
	if (child_pid[1] == -1)
		error_exit("fork");
	if (child_pid[1] == 0)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
			error_exit("dup2");
		close(pipefd[0]);
		close(pipefd[1]);
		exit(executor(node->right, sh));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(child_pid[0], &status[0], 0);
	waitpid(child_pid[1], &status[1], 0);
	if (WIFEXITED(status[1]))
		exit_status = WEXITSTATUS(status[1]);
	else
		exit_status = EXIT_FAILURE;
	return (exit_status);
}
