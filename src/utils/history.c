# include "../../includes/minishell.h"

/* Get history file path */
char	*get_history_path(void )
{
	char *home;

	home = getenv("HOME");
	if (!home)
		return (NULL);
	return (ft_strjoin(home, HISTFILE));
}

void load_history(t_shell *shell)
{
	char	*path;
	int		fd;
	int		histfile_lines_c;
	int		skip[2];

	path = get_history_path();
	if (!path)
		return ;
	histfile_lines_c = get_histfile_lines_count(path, O_RDONLY, 0);
	if (histfile_lines_c == -1)
		return ;
	// Skip n == skip[0] lines to get to the first line out of histfilesize
	skip[0] = histfile_lines_c - shell->history.histfilesize;
	if (skip[0] <= 0)
		skip[0] = 0;
	// Skip n == skip[1] lines to get to the first line to load to in-memory history list
	skip[1] = histfile_lines_c - shell->history.histsize;
    if (skip[1] <= 0)
		skip[1] = 0;
	fd = open(path, O_RDONLY);
	free(path);
	if (fd == -1)
		return ;
	load_history_fd(shell, histfile_lines_c, skip, fd);
	if (shell->history.count)
		shell->history.current = shell->history.count;
    close(fd);
}

/* Open HISTORY_FILE with Truncate OR Append Mode, then save in-memory history to HISTORY_FILE */
void	save_history_oflag(char *path, int oflag, t_shell *shell, int skip)
{
	int	fd;
	int	i;

	fd = open(path, oflag, 0644);
	if (fd == -1)
	{
		perror("Minishell: error");
		return ;
	}
	i = 0;
	while (i < shell->history.histmem_lines_c && i < shell->history.histfilesize)
	{
		ft_putstr_fd(shell->history.entries[shell->history.current + i], fd);
		if (i >= skip)
			ft_putchar_fd('\n', fd);
		i++;
	}
	close(fd);
}

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
	histfile_lines_c = 0;
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
	}
}

/* Add input to in-memory history list*/
void add_to_history(t_shell *shell, char *input)
{
	int		i;

	if (!input || (input && !*input))
		return ;
	// Rotate history if full
	else if (shell->history.histsize - (shell->history.histfilesize - shell->history.count)
			== shell->history.histsize)
	{
		rl_clear_history();
		free(shell->history.entries[0]);
		ft_memmove(shell->history.entries,
					shell->history.entries + 1,
					shell->history.count * sizeof(char *));
		shell->history.count--;
		if (shell->history.current)
			shell->history.current--;
		i = 0;
		while (i < shell->history.count)
			add_history(shell->history.entries[i++]);
	}
	add_history(input);
	shell->history.entries[shell->history.count++] = ft_strdup(input);
}
