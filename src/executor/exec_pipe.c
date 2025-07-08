/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:41:10 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/08 17:08:24 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	error_exit(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

static void	dup2_and_exec(t_ast *node, t_shell *sh, int src_fd, int pipefd[2])
{
	int	dest_fd;

	dest_fd = STDOUT_FILENO;
	if (pipefd[0] == src_fd)
		dest_fd = STDIN_FILENO;
	if (dup2(src_fd, dest_fd) == -1)
		error_exit("dup2");
	close(pipefd[0]);
	close(pipefd[1]);
	exit(executor(node, sh));
}

int	exec_pipe(t_ast *node, t_shell *sh)
{
	int		pipefd[2];
	pid_t	child_pid[2];
	int		status;

	if (!node || !node->left || !node->right)
		return (EXIT_FAILURE);
	if (pipe(pipefd) == -1)
		return (perror("pipe"), 1);
	child_pid[0] = fork();
	if (child_pid[0] == -1)
		error_exit("fork");
	if (child_pid[0] == 0)
		dup2_and_exec(node->left, sh, pipefd[1], pipefd);
	child_pid[1] = fork();
	if (child_pid[1] == -1)
		error_exit("fork");
	if (child_pid[1] == 0)
		dup2_and_exec(node->right, sh, pipefd[0], pipefd);
	close(pipefd[0]);
	close(pipefd[1]);
	if (waitpid(child_pid[1], &status, 0) != -1 && WIFEXITED(status))
		return (WEXITSTATUS(status));
	waitpid(child_pid[0], NULL, 0);
	return (EXIT_FAILURE);
}
