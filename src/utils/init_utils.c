/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 19:20:22 by mel-hami          #+#    #+#             */
/*   Updated: 2025/07/09 19:20:25 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	increment_shell_level(t_list **vars)
{
	t_list	*shlvl;
	char	*str_lvl;
	int		lvl;

	shlvl = find_shell_var(*vars, "SHLVL");
	if (!shlvl)
		create_shell_var(vars, "SHLVL", "1", VAR_ENV);
	else
	{
		lvl = ft_atoi(((t_var *)shlvl->content)->value);
		str_lvl = ft_itoa(++lvl);
		update_shell_var(shlvl, str_lvl, ((t_var *)shlvl->content)->flag);
		free(str_lvl);
	}
}

void	init_shell_vars(t_list **vars)
{
	char	*cwd;

	if (!getenv("PATH"))
		create_shell_var(vars, "PATH", \
"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", VAR_LOCAL);
	if (!getenv("PWD"))
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			create_shell_var(vars, "PWD", cwd, VAR_ENV);
			free(cwd);
		}
	}
	increment_shell_level(vars);
}

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

char	*_retreive_home(__uid_t uid)
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
