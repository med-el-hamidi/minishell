/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:24:01 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 16:24:02 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

static int	count_lines_in_file(int fd)
{
	ssize_t	bytes;
	int		count;
	char	c;

	if (fd < 0)
		return (0);
	count = 0;
	bytes = 1;
	while (bytes)
	{
		bytes = read(fd, &c, 1);
		if (bytes == -1)
		{
			close(fd);
			return (-1);
		}
		else if (!bytes)
			break ;
		if (c == '\n')
			count++;
	}
	return (count);
}

int	get_histfile_lines_c(char *path, int oflag, int perm)
{
	int	fd;
	int	histfile_lines_c;

	if (perm)
		fd = open(path, oflag, perm);
	else
		fd = open(path, oflag);
	if (fd == -1)
		return (-1);
	histfile_lines_c = count_lines_in_file(fd);
	close(fd);
	return (histfile_lines_c);
}

void	load_hist_fd(t_shell *shell, int histfile_lines_c, int *skip, int fd)
{
	char	*line;
	int		i;

	i = -1;
	while (++i < histfile_lines_c)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (i >= skip[0])
		{
			if (!_add_history(shell, line, i, skip))
				free(line);
		}
		else
			free(line);
	}
}

static void	get_recent_hist_fd(t_shell *s, int e, int histfile_lines_c, int *fd)
{
	char	*line;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (i < histfile_lines_c)
	{
		line = get_next_line(*fd);
		if (!line)
			break ;
		if (i >= e && j < s->history.histfilesize)
		{
			if (s->history.entries[j])
				free(s->history.entries[j]);
			s->history.entries[j++] = line;
			s->history.count++;
		}
		else
			free(line);
		i++;
	}
}

int	load_recent_history(char *path, t_shell *shell, int histfile_lines_c)
{
	int		fd;
	int		start;
	int		exceed;

	fd = open(path, O_CREAT | O_RDWR, 0600);
	if (fd == -1)
		return (0);
	start = 0;
	if (histfile_lines_c > shell->history.histfilesize)
		start = histfile_lines_c - shell->history.histfilesize;
	exceed = shell->history.histmem_lines_c + start;
	if (shell->history.current != \
		(shell->history.histfilesize - shell->history.histmem_lines_c))
		ft_memmove(shell->history.entries + \
			(shell->history.histfilesize - shell->history.histmem_lines_c),
			shell->history.entries + shell->history.current,
			shell->history.histmem_lines_c * sizeof(char *));
	get_recent_hist_fd(shell, exceed, histfile_lines_c, &fd);
	close(fd);
	return (histfile_lines_c - exceed);
}
