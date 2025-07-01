#include "../../includes/minishell.h"

int	exec_redirection(t_ast *node, t_shell *shell)
{
	int saved_stdin;
	int saved_stdout;
	int ret;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (!node || !node->redir_file)
		return (1);
	if (node->redir_type == REDIR_INPUT || node->redir_type == REDIR_HEREDOC)
		node->redir_fd = open(node->redir_file, O_RDONLY);
	else if (node->redir_type == REDIR_OUTPUT)
		node->redir_fd = open(node->redir_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (node->redir_type == REDIR_APPEND)
		node->redir_fd = open(node->redir_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (node->redir_fd == -1)
		return (perror("minishell: redirection open"),1);
	if (node->redir_type == REDIR_INPUT || node->redir_type == REDIR_HEREDOC)
		dup2(node->redir_fd, STDIN_FILENO);
	else
		dup2(node->redir_fd, STDOUT_FILENO);
	close (node->redir_fd);
	ret = executor(node->left, shell);

	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (ret);
}
