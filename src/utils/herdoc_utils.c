/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:48:23 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 15:48:24 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*_gen_tmp_file(void )
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

static char	*_parse_herdoc_input(t_shell *shell, char *input, int f)
{
	char	*result;
	size_t	i;

	result = ft_strdup("");
	i = 0;
	while (input[i])
	{
		if (f && input[i] == '$' && input[i + 1] != '"' && input[i + 1] != '\'')
			result = \
			ft_strjoin_to_s1(result, accu_dollar(shell, input, &i, _getenv_al));
		else
			result = ft_strjoin_char_to_s1(result, input[i++]);
	}
	return (free(input), result);
}

static void	_herdoc_loop(t_shell *shell, char *delimiter, int fd, int f)
{
	const size_t	len = ft_strlen(delimiter);
	char			*input;

	signal(SIGINT, SIG_DFL);
	while (1)
	{
		input = readline("> ");
		if (!input || (!ft_strncmp(input, delimiter, len)
				&& ft_strlen(input) == len))
		{
			if (!input)
				printf("minishell: warning: here-document delimited by end-of-file \
(wanted '%s')\n", delimiter);
			break ;
		}
		input = _parse_herdoc_input(shell, input, f);
		ft_putendl_fd(input, fd);
		free(input);
	}
	exit(0);
}

static int	_ignore_all_heredocs(char *tmp_file, int *fd, int status)
{
	int	ret;

	ret = 128 + status;
	close (*fd);
	*fd = open(tmp_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (*fd == -1)
	{
		perror("minishell: herdoc");
		return (1);
	}
	return (ret);
}

int	set_herdoc_tmp_file(t_shell *shell, char **delimiter, int parse)
{
	char	*tmp_file;
	int		fd;
	pid_t	pid;
	int		status;
	int		ret;

	ret = 0;
	tmp_file = _gen_tmp_file();
	fd = open(tmp_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (fd == -1)
		return (perror("minishell: herdoc"), 1);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (!pid)
		_herdoc_loop(shell, *delimiter, fd, parse);
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFSIGNALED(status) && WTERMSIG(status))
		ret = _ignore_all_heredocs(tmp_file, &fd, status);
	(free(*delimiter), close (fd));
	return ((*delimiter = tmp_file), ret);
}
