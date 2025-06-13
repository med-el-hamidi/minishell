#include "../../includes/minishell.h"

void	set_herdoc_tmp_file(t_ast	*ast)
{
	char	*tmp_file;
	char	*pid;
	char	*input;
	int		len;
	int		fd;

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
		if (!ft_strncmp(input, ast->redir_file, len))
			break ;
		ft_putendl_fd(input, fd);
		free(input);
	}
	close(fd);
	free(ast->redir_file);
	ast->redir_file = tmp_file;
}
