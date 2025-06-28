#include "../../includes/minishell.h"

static char	*gen_tmp_file(void )
{
	static int	uniq_id = 0;
	char		*str;
	char		*tmp;
	char		*tmp_file;

	str = _getpid();
	tmp_file = ft_strjoin("/tmp/minishell_herdoc_", str);
	free(str);
	str = ft_itoa(uniq_id++);
	tmp = tmp_file;
	tmp_file = ft_strjoin(tmp_file, str);
	free(tmp);
	free(str);
	return (tmp_file);
}

void	set_herdoc_tmp_file(t_ast	*ast)
{
	char		*tmp_file;
	char		*input;
	int			fd;
	size_t		len;

	tmp_file = gen_tmp_file();
	len = ft_strlen(ast->redir_file);
	fd = open(tmp_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (fd == -1)
		perror("minishell: herdoc");
	while (1)
	{
		input = readline("> ");
		if (!input)
		{
			printf("minishell: warning: here-document delimited by end-of-file (wanted '%s')\n", ast->redir_file);
			break ;
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
