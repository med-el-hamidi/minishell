/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   histfile_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 02:24:50 by mel-hami          #+#    #+#             */
/*   Updated: 2025/07/13 02:24:51 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	_getuid(void )
{
	char	*buf;
	char	*str_uid;
	int		uid;
	int		fd;

	str_uid = NULL;
	fd = open("/proc/self/status", O_RDONLY);
	if (fd == -1)
		return (perror("minishell: getpid"), 0);
	while (1)
	{
		buf = get_next_line(fd);
		if (!buf)
			break ;
		else if (!ft_strncmp(buf, "Uid:", 4))
			str_uid = \
				ft_substr(buf, ft_strchr(buf, ':') - buf + 1, ft_strlen(buf));
		free(buf);
	}
	close(fd);
	if (!str_uid)
		return (0);
	uid = ft_atoi(str_uid);
	free(str_uid);
	return (uid);
}

static char	*_retreive_home(__uid_t uid)
{
	char			*home;
	DIR				*dir;
	struct dirent	*entry;
	struct stat		state;

	dir = opendir("/home");
	if (!dir)
		return (NULL);
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (!ft_strncmp(entry->d_name, ".", 1))
			continue ;
		home = ft_strjoin("/home/", entry->d_name);
		stat(home, &state);
		if (S_ISDIR(state.st_mode) && uid == state.st_uid)
			break ;
		free(home);
	}
	closedir(dir);
	return (home);
}

void	set_histfile(t_shell *shell)
{
	char	*home;

	shell->history.path = NULL;
	home = NULL;
	if (getenv("HOME"))
		home = ft_strdup(getenv("HOME"));
	else
		home = _retreive_home(_getuid());
	if (home)
	{
		shell->history.path = ft_strjoin(home, HISTFILE);
		create_shell_var(&shell->vars, "HISTFILE", \
			shell->history.path, VAR_LOCAL);
		free(home);
	}
}

void	set_default_history_sizes(t_list **vars, char *key, int n)
{
	char	*nbr;

	nbr = ft_itoa(n);
	create_shell_var(vars, key, nbr, VAR_LOCAL);
	free(nbr);
}

int	_add_history(t_shell *shell, char *line, int i, int *skip)
{
	int	len;

	len = ft_strlen(line);
	if (len > 1 && line[len - 1] == '\n')
	{
		line[len - 1] = '\0';
		if (i > skip[1] || !skip[1])
			add_history(line);
		shell->history.entries[shell->history.count++] = line;
		return (1);
	}
	return (0);
}
