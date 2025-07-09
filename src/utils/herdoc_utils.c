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

static char	*parse_herdoc_input(t_shell *shell, char *input)
{
	char	*result;
	size_t	i;

	result = ft_strdup("");
	i = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] != '"' && input[i + 1] != '\'')
			result = \
				ft_strjoin_to_s1(result, accumulate_dollar(shell, input, &i));
		else
			result = ft_strjoin_char_to_s1(result, input[i++]);
	}
	return (free(input), result);
}

static void	herdoc_loop(t_shell *shell, char *delimiter, int fd)
{
	const size_t	len = ft_strlen(delimiter);
	char			*input;

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
		input = parse_herdoc_input(shell, input);
		ft_putendl_fd(input, fd);
		free(input);
	}
}

void	set_herdoc_tmp_file(t_shell *shell, char **delimiter)
{
	char			*tmp_file;
	int				fd;
	pid_t			pid;
	int				status;

	tmp_file = gen_tmp_file();
	fd = open(tmp_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
	if (fd == -1)
		return (perror("minishell: herdoc"));
	pid = fork();
	if (!pid)
	{
		signal(SIGINT, SIG_DFL);
		(herdoc_loop(shell, *delimiter, fd), exit(EXIT_SUCCESS));
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status))
	{
		close (fd);
		fd = open(tmp_file, O_CREAT | O_TRUNC | O_WRONLY, 0600);
		if (fd == -1)
			return (perror("minishell: herdoc"));
	}
	(free(*delimiter), close (fd));
	*delimiter = tmp_file;
}
