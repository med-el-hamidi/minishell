#include "../../includes/minishell.h"

int	exec_redirection(t_ast *node, t_shell *shell)
{
	int	fd = -1;

	if (!node || node->type != AST_REDIR || !node->redir_file)
		return (1);
	if (node->redir_type == REDIR_INPUT || node->redir_type == REDIR_HEREDOC)
		fd = open(node->redir_file, O_RDONLY);
	else if (node->redir_type == REDIR_OUTPUT)
		fd = open(node->redir_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->redir_type == REDIR_APPEND)
		fd = open(node->redir_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("redirection open");
		return (1);
	}
	if (node->redir_type == REDIR_INPUT || node->redir_type == REDIR_HEREDOC)
		dup2(fd, STDIN_FILENO);
	else
		dup2(fd, STDOUT_FILENO);
	close (fd);
	return (executor(node->left, shell));
}
