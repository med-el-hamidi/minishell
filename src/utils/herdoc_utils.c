#include "../../includes/minishell.h"

void	set_herdoc_tmp_file(t_ast	*ast)
{
	char	*tmp_file;
	char	*pid;
	char	*input;
	int		fd;
	size_t	len;

	pid = _getpid();
	tmp_file = ft_strjoin("/tmp/minishell_herdoc_", pid);
	free(pid);
	len = ft_strlen(ast->redir_file);
	fd = open(tmp_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (fd == -1)
		return (perror("minishell: herdoc"));
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			printf("minishell: warning: here-document delimited by end-of-file (wanted '%s')\n", ast->redir_file);
			break;
		}
		else if (!ft_strncmp(input, ast->redir_file, len) && ft_strlen(input) == len)
			break ;
		ft_putendl_fd(input, fd);
		free(input);
	}
	free(ast->redir_file);
	ast->redir_file = tmp_file;
	ast->redir_fd = fd;
}
