/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:48:34 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 20:48:35 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	open_script(char *script)
{
	int	fd;

	fd = open(script, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (script && !*script)
			perror(" ");
		else
			perror(script);
		exit(127);
	}
	return (fd);
}

char	*_getenv(t_shell *shell, const char *name)
{
	t_list	*ptr;
	t_var	*env;
	size_t	len;

	len = ft_strlen(name);
	ptr = shell->vars;
	while (ptr)
	{
		env = ptr->content;
		if (!ft_strncmp(env->key, name, len) && ft_strlen(env->key) == len)
			return (env->value);
		ptr = ptr->next;
	}
	return (NULL);
}

char	*_getpid(void )
{
	char	buf[11];
	ssize_t	bytes;
	int		fd;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: getpid");
		return (NULL);
	}
	bytes = read(fd, buf, 10);
	close(fd);
	if (bytes == -1)
	{
		perror("minishell: getpid");
		return (NULL);
	}
	else if (!bytes)
		return (NULL);
	return (ft_itoa(ft_atoi(buf)));
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
