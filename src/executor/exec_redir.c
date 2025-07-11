/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:46:27 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/09 20:50:45 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_redir_file(t_ast *node)
{
	if (!node || !node->redir_file)
		return (2);
	if (node->redir_type == REDIR_INPUT || node->redir_type == REDIR_HEREDOC)
		node->redir_fd = open(node->redir_file, O_RDONLY);
	else if (node->redir_type == REDIR_OUTPUT)
		node->redir_fd = open(node->redir_file, \
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->redir_type == REDIR_APPEND)
		node->redir_fd = open(node->redir_file, \
			O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (node->redir_fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (node->redir_file && node->redir_file[0] != '\0')
			return (perror(node->redir_file), 1);
		return (perror(" "), 1);
	}
	return (0);
}

int	exec_redirection(t_ast *node, t_shell *shell)
{
	int	exit_status;
	int	f_stdin;
	int	f_stdout;

	f_stdin = 0;
	f_stdout = 0;
	while (node && node->type == AST_REDIR)
	{
		exit_status = open_redir_file(node);
		if (exit_status)
			break ;
		if (!f_stdin && (node->redir_type == REDIR_INPUT || \
			node->redir_type == REDIR_HEREDOC))
			{
				dup2(node->redir_fd, STDIN_FILENO);
				f_stdin = 1;
			}
		else if (!f_stdout)
		{
			dup2(node->redir_fd, STDOUT_FILENO);
			f_stdout = 1;
		}
		close(node->redir_fd);
		node = node->left;
	}
	if (!exit_status)
		exit_status = executor(node, shell);
	(dup2(shell->stdin_fd, STDIN_FILENO), dup2(shell->stdout_fd, STDOUT_FILENO));
	return (exit_status);
}
