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

#include "../../includes_bonus/minishell_bonus.h"

static char	*_gen_tmp_file(void )
{
	static int	uniq_id = 0;
	char		*tmp_file;
	char		*addr_str;
	char		*tmp;
	uintptr_t	addr;

	addr = (uintptr_t)(&tmp_file);
	addr_str = ft_itoa((int)addr);
	tmp = ft_strjoin("/tmp/minishell_heredoc_", addr_str);
	tmp_file = ft_strjoin(tmp, "_");
	tmp_file = ft_strjoin_to_s1(tmp_file, ft_itoa(uniq_id++));
	free(addr_str);
	free(tmp);
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

static int	_ignore_all_heredocs(char *tmp_file, int *fd)
{
	close (*fd);
	*fd = open(tmp_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (*fd == -1)
		perror("minishell: herdoc");
	return (1);
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
		ret = _ignore_all_heredocs(tmp_file, &fd);
	(free(*delimiter), close (fd));
	return ((*delimiter = tmp_file), ret);
}
