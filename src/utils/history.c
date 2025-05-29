# include "../../includes/minishell.h"

/* Get history file path */
static char	*get_history_path(void )
{
	char *home = getenv("HOME");
	if (!home)
		return (ft_strjoin("~", HISTORY_FILE));;
	return (ft_strjoin(home, HISTORY_FILE));
}

void load_history(t_shell *shell)
{
	int		fd;
	int		histfile_lines_c;
	int		skip[2];
	char	*path;

	path = get_history_path();
	if (!path)
		return ;
	histfile_lines_c = get_histfile_lines_count(path, O_RDONLY, 0);
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
void	save_history_oflag(char *path, int oflag, t_shell *shell, int histmem_lines_c)
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
	while (i < histmem_lines_c && i < shell->history.histfilesize)
	{
		ft_putstr_fd(shell->history.entries[shell->history.current + i], fd);
		ft_putchar_fd('\n', fd);
		i++;
	}
	close(fd);
}

/*  Save the history */
void save_history(t_shell *shell)
{
	char	*path;
	int		histfile_lines_c;
	int		histmem_lines_c;  // Number of inputs in minishell session

	path = get_history_path();
	histmem_lines_c = shell->history.count - shell->history.current;
	if (!path || !histmem_lines_c)
	{
		free(path);
		return;
    }
	if (histmem_lines_c != shell->history.histfilesize)
		histfile_lines_c = get_histfile_lines_count(path, O_CREAT | O_RDONLY, 0644);

	// Save all history from in-memory history list to HISTORY_FILE after truncate it.
	if (histmem_lines_c == shell->history.histfilesize)
		save_history_oflag(path, O_CREAT | O_WRONLY | O_TRUNC, shell, histmem_lines_c); /* Save in-memory history to HISTORY_FILE after truncate it */

	// Append all history from in-memory history list to HISTORY_FILE
	if (histmem_lines_c + histfile_lines_c < shell->history.histfilesize)
		save_history_oflag(path, O_CREAT | O_WRONLY | O_APPEND, shell, histmem_lines_c); /* Append in-memory history to HISTORY_FILE */
	else
	{
		// Our history (in-memory + file)  exceed histfilesize, so we load_recent_history from file into in-memory list in case the history file changed,
		load_recent_history(path, shell, histmem_lines_c, histfile_lines_c);
		shell->history.current = 0;
		histmem_lines_c = shell->history.count;
		save_history_oflag(path, O_CREAT | O_WRONLY | O_TRUNC, shell, histmem_lines_c); /* Save in-memory history to HISTORY_FILE after truncate it */
	}
	free(path);
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
		i = 0;
		free(shell->history.entries[0]);
		ft_memmove(shell->history.entries,
					shell->history.entries + 1,
					shell->history.count * sizeof(char *));
		shell->history.count--;
		if (shell->history.current)
			shell->history.current--;
		while (i < shell->history.count)
			add_history(shell->history.entries[i++]);
	}
	add_history(input);
	shell->history.entries[shell->history.count++] = ft_strdup(input);
}
