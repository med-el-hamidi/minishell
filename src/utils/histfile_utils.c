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

static uid_t	_getuid(t_shell *shell)
{
	char			*pwd;
	DIR				*dir;
	struct dirent	*entry;
	struct stat		state;

	pwd = _getenv(shell->vars, "PWD");
	if (!pwd)
		return (0);
	dir = opendir(pwd);
	if (!dir)
		return (free(pwd), 0);
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			return (closedir(dir), free(pwd), 0);
		if (ft_strcmp(entry->d_name, shell->name))
			continue ;
		pwd = ft_strjoin_to_s1(pwd, ft_strjoin("/", entry->d_name));
		if (!stat(pwd, &state))
			return (closedir(dir), free(pwd), state.st_uid);
		break ;
	}
	return (closedir(dir), free(pwd), 0);
}

static char	*_retreive_home(uid_t	uid)
{
	char			*home;
	DIR				*dir;
	struct dirent	*entry;
	struct stat		state;

	if (!uid)
		return (NULL);
	dir = opendir("/mnt/homes");
	if (!dir)
		return (NULL);
	while (1)
	{
		entry = readdir(dir);
		if (!entry)
			break ;
		if (!ft_strncmp(entry->d_name, ".", 1))
			continue ;
		home = ft_strjoin("/mnt/homes/", entry->d_name);
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
	char		*home;

	shell->history.path = NULL;
	home = NULL;
	if (getenv("HISTFILE"))
		shell->history.path = ft_strdup(getenv("HISTFILE"));
	else if (getenv("HOME"))
		home = ft_strdup(getenv("HOME"));
	else
		home = _retreive_home(_getuid(shell));
	if (home)
	{
		shell->history.path = ft_strjoin(home, HISTFILE);
		free(home);
	}
	if (shell->history.path)
		create_shell_var(&shell->vars, "HISTFILE", \
			shell->history.path, VAR_LOCAL);
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
