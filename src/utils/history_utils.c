# include "../../includes/minishell.h"

/* Count number of lines in history file */
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
			exit_error("History is not initialized!: Cannot read from ~"HISTORY_FILE, 1);
		}
		else if (!bytes)
			break ;
		if (c == '\n')
			count++;
	}
	return (count);
}

/* Get number of lines in history file */
int	get_histfile_lines_count(char *path, int oflag, int perm)
{
	int	fd;
	int	histfile_lines_c;

	if (perm)
		fd = open(path, oflag, perm);
	else
		fd = open(path, oflag);
	if (fd == -1)
	{
		free(path);
		exit_error("History is not initialized!: Cannot read from ~"HISTORY_FILE, 1);
	}
	histfile_lines_c = count_lines_in_file(fd);
	close(fd);
	return (histfile_lines_c);
}

void	load_history_fd(t_shell *shell, int	histfile_lines_c, int *skip, int fd)
{
	char	*line;
	size_t	len;
	int		i;

	// Using get_next_line to read history file
	i= -1;
	while (++i < histfile_lines_c)
    {
		line = get_next_line(fd);
		if (!line)
        	break ;
		if (i >= skip[0])
		{
			len = ft_strlen(line);
			if (len > 1 && line[len - 1] == '\n')
			{
				// Remove trailing newline if present
				line[len - 1] = '\0';
				if (i > skip[1] || !skip[1])
					add_history(line);
				shell->history.entries[shell->history.count++] = line;
			}
			else
				free(line);
		}
		else
			free(line);
    }
}

/* Get the recent lines from HISTORY_FILE from "exceed" (Look at load_recent_history)*/
static void	get_recent_history_fd(t_shell *shell, int exceed, int histfile_lines_c, int *fd)
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
		if (i >= exceed && j < shell->history.histfilesize)
		{
			if (shell->history.entries[j])
				free(shell->history.entries[j]);
			shell->history.entries[j++] = line;
			shell->history.count++;
		}
		else
			free(line);
		i++;
	}
}

/* Load the recent history from HISTORY_FILE*/
int	load_recent_history(char *path, t_shell *shell, int histfile_lines_c)
{
	int		fd;
	int		start;
	int		exceed;

	fd = open(path, O_CREAT | O_RDWR, 0644);
	if (fd == -1)
	{
		perror("Minishell: error");
		return (0);
	}
	start = 0;
	// In case recent HISTORY_FILE has more that histfilesize,
	// We concider the recent HISTORY_FILE starts from index "start" -> histfilesize (The limit we concider from env)
	if (histfile_lines_c > shell->history.histfilesize)
		start = histfile_lines_c - shell->history.histfilesize;
	// Calculate the exceed part from the HISTORY_FILE out of histfilesize
	exceed =  shell->history.histmem_lines_c + start;
	// Move our in-memory history into its position (histfilesize - shell->history.histmem_lines_c) in entries,
	// To let the first entris to lines from HISTORY_FILE from "exceed"
	if (shell->history.current != (shell->history.histfilesize - shell->history.histmem_lines_c))
		ft_memmove(shell->history.entries + (shell->history.histfilesize - shell->history.histmem_lines_c),
					shell->history.entries + shell->history.current,
					shell->history.histmem_lines_c * sizeof(char *));
	get_recent_history_fd(shell, exceed, histfile_lines_c, &fd);
	close(fd);
	return (histfile_lines_c - exceed);
}
