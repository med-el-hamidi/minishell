/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 15:56:39 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 15:56:40 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	load_history(t_shell *shell)
{
	int		fd;
	int		histfile_lines_c;
	int		skip[2];

	histfile_lines_c = get_histfile_lines_c(shell->history.path, O_RDONLY, 0);
	if (histfile_lines_c == -1)
		return ;
	skip[0] = histfile_lines_c - shell->history.histfilesize;
	if (skip[0] <= 0)
		skip[0] = 0;
	skip[1] = histfile_lines_c - shell->history.histsize;
	if (skip[1] <= 0)
		skip[1] = 0;
	fd = open(shell->history.path, O_RDONLY);
	if (fd == -1)
		return ;
	load_hist_fd(shell, histfile_lines_c, skip, fd);
	if (shell->history.count)
		shell->history.current = shell->history.count;
	close(fd);
}

void	save_history_oflag(char *path, int oflag, t_shell *shell, int skip)
{
	int	fd;
	int	i;

	fd = open(path, oflag, 0600);
	if (fd == -1)
	{
		perror("Minishell: history: cannot save history in ~"HISTFILE);
		return ;
	}
	i = 0;
	while (i < shell->history.histmem_lines_c
		&& i < shell->history.histfilesize)
	{
		ft_putstr_fd(shell->history.entries[shell->history.current + i], fd);
		if (i >= skip)
			ft_putchar_fd('\n', fd);
		i++;
	}
	close(fd);
}

void	save_history(t_shell *shell, char *path)
{
	int			histfile_lines_c;
	int			skip;
	const int	histmem_lines_c = shell->history.count - shell->history.current;

	if (!shell || !shell->history.entries || !path)
		return ;
	shell->history.histmem_lines_c = histmem_lines_c;
	if (!shell->history.histmem_lines_c)
		return ;
	histfile_lines_c = 0;
	if (shell->history.histmem_lines_c != shell->history.histfilesize)
		histfile_lines_c = get_histfile_lines_c(path, O_CREAT | O_RDONLY, 0600);
	if (histfile_lines_c == -1)
		return ;
	if (shell->history.histmem_lines_c == shell->history.histfilesize)
		save_history_oflag(path, O_CREAT | O_WRONLY | O_TRUNC, shell, 0);
	else if (histmem_lines_c + histfile_lines_c < shell->history.histfilesize)
		save_history_oflag(path, O_CREAT | O_WRONLY | O_APPEND, shell, 0);
	else if (!access(path, R_OK))
	{
		skip = load_recent_history(path, shell, histfile_lines_c);
		shell->history.current = 0;
		shell->history.histmem_lines_c = shell->history.count;
		save_history_oflag(path, O_CREAT | O_WRONLY | O_TRUNC, shell, skip);
	}
}

void	add_to_history(t_shell *shell, char *input)
{
	int	i;

	if (!input || (input && !*input))
		return ;
	else if (shell->history.histfilesize == shell->history.count)
	{
		free(shell->history.entries[0]);
		ft_memmove(shell->history.entries,
			shell->history.entries + 1,
			shell->history.count * sizeof(char *));
		shell->history.count--;
		if (shell->history.current)
			shell->history.current--;
	}
	shell->history.entries[shell->history.count++] = ft_strdup(input);
	if (shell->history.histsize <= shell->history.count)
	{
		rl_clear_history();
		i = shell->history.count - shell->history.histsize + 1;
		while (i < shell->history.count)
			add_history(shell->history.entries[i++]);
	}
	add_history(input);
}
