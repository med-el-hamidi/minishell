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

<<<<<<< HEAD
/* Get history file path */
char	*get_history_path(void )
{
	char *home;

	home = getenv("HOME");
	if (!home)
		return (NULL);
	return (ft_strjoin(home, HISTFILE));
}
=======
#include "../../includes/minishell.h"
>>>>>>> parser

void	load_history(t_shell *shell)
{
	char	*path;
	int		fd;
	int		histfile_lines_c;
	int		skip[2];

	histfile_lines_c = get_histfile_lines_c(shell->history.path, O_RDONLY, 0);
	if (histfile_lines_c == -1)
		return ;
<<<<<<< HEAD
	histfile_lines_c = get_histfile_lines_count(path, O_RDONLY, 0);
	if (histfile_lines_c == -1)
		return ;
	// Skip n == skip[0] lines to get to the first line out of histfilesize
=======
>>>>>>> parser
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

<<<<<<< HEAD
/* Open HISTORY_FILE with Truncate OR Append Mode, then save in-memory history to HISTORY_FILE */
=======
>>>>>>> parser
void	save_history_oflag(char *path, int oflag, t_shell *shell, int skip)
{
	int	fd;
	int	i;

	fd = open(path, oflag, 0600);
	if (fd == -1)
	{
		perror("Minishell: history: cannot save history!");
		return ;
	}
	i = 0;
<<<<<<< HEAD
	while (i < shell->history.histmem_lines_c && i < shell->history.histfilesize)
=======
	while (i < shell->history.histmem_lines_c
		&& i < shell->history.histfilesize)
>>>>>>> parser
	{
		ft_putstr_fd(shell->history.entries[shell->history.current + i], fd);
		if (i >= skip)
			ft_putchar_fd('\n', fd);
		i++;
	}
	close(fd);
}

<<<<<<< HEAD
/*  Save the history */
void save_history(t_shell *shell, char *path)
{
	int		histfile_lines_c;
	int		skip;

	if (!shell || !shell->history.entries || !path)
		return ;
	shell->history.histmem_lines_c = shell->history.count - shell->history.current;
	if (!shell->history.histmem_lines_c)
		return ;
	if (shell->history.histmem_lines_c != shell->history.histfilesize)
		histfile_lines_c = get_histfile_lines_count(path, O_CREAT | O_RDONLY, 0644);
	if (histfile_lines_c == -1)
		return ;

	// Save all history from in-memory history list to HISTORY_FILE after truncate it.
	if (shell->history.histmem_lines_c == shell->history.histfilesize)
		save_history_oflag(path, O_CREAT | O_WRONLY | O_TRUNC, shell, 0); /* Save in-memory history to HISTORY_FILE after truncate it */

	// Append all history from in-memory history list to HISTORY_FILE
	else if (shell->history.histmem_lines_c + histfile_lines_c < shell->history.histfilesize)
		save_history_oflag(path, O_CREAT | O_WRONLY | O_APPEND, shell, 0); /* Append in-memory history to HISTORY_FILE */
	else if (!access(path, R_OK))
	{
		// Our history (in-memory + file)  exceed histfilesize, so we load_recent_history from file into in-memory list in case the history file changed,
		skip = load_recent_history(path, shell, histfile_lines_c);
		shell->history.current = 0;
		shell->history.histmem_lines_c = shell->history.count;
		save_history_oflag(path, O_CREAT | O_WRONLY | O_TRUNC, shell, skip); /* Save in-memory history to HISTORY_FILE after truncate it */
=======
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
>>>>>>> parser
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
