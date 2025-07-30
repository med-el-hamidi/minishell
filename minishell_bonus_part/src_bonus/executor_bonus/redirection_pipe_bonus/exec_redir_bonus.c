/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 17:46:27 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/30 21:43:57 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes_bonus/minishell_bonus.h"

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
	else if (node->redir_type == REDIR_NONE)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->redir_file, STDERR_FILENO);
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
		return (1);
	}
	if (node->redir_fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (node->redir_file && node->redir_file[0] != '\0')
			return (perror(node->redir_file), 1);
		return (perror(" "), 1);
	}
	return (0);
}

static int	_redir_count(t_ast *node)
{
	t_ast	*ptr;
	size_t	len;

	ptr = node;
	len = 0;
	while (ptr && ptr->type == AST_REDIR)
	{
		len++;
		ptr = ptr->left;
	}
	return (len);
}

static void	_open_redir_file_wraper(t_ast *node, int *exit_status)
{
	t_ast	*ptr;
	size_t	len;
	size_t	i;

	len = _redir_count(node);
	while (len)
	{
		ptr = node;
		i = 0;
		while (ptr && ptr->type == AST_REDIR)
		{
			if (++i == len)
			{
				*exit_status = open_redir_file(ptr);
				if (*exit_status)
					break ;
			}
			ptr = ptr->left;
		}
		if (*exit_status)
			break ;
		len--;
	}
}

static void	_dup2_leading_redir_fds(t_ast *node, int *f_stdin, int *f_stdout)
{
	if (!*f_stdin && node->redir_fd != -1 && \
		(node->redir_type == REDIR_INPUT
			|| node->redir_type == REDIR_HEREDOC))
	{
		dup2(node->redir_fd, STDIN_FILENO);
		*f_stdin = 1;
	}
	else if (!*f_stdout && node->redir_fd != -1
		&& (node->redir_type == REDIR_OUTPUT
			|| node->redir_type == REDIR_APPEND))
	{
		dup2(node->redir_fd, STDOUT_FILENO);
		*f_stdout = 1;
	}
	if (node->redir_fd != -1)
		close(node->redir_fd);
}

int	exec_redirection(t_ast *node, t_shell *shell)
{
	int		exit_status;
	int		f_stdin;
	int		f_stdout;

	f_stdin = 0;
	f_stdout = 0;
	exit_status = 0;
	_open_redir_file_wraper(node, &exit_status);
	while (node && node->type == AST_REDIR)
	{
		_dup2_leading_redir_fds(node, &f_stdin, &f_stdout);
		node = node->left;
	}
	if (!exit_status)
		exit_status = executor(node, shell);
	dup2(shell->stdin_fd, STDIN_FILENO);
	dup2(shell->stdout_fd, STDOUT_FILENO);
	return (exit_status);
}
