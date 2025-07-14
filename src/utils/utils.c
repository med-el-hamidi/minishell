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

static char	*_getenv_helper(char *value)
{
	char	**content;
	size_t	i;

	if (!has_whitespace(value))
		return (ft_strdup(value));
	content = ft_split_set(value, " \t");
	if (!content)
		return (NULL);
	value = NULL;
	i = 0;
	while (content[i])
	{
		value = ft_strjoin_to_s1(value, content[i]);
		if (content[i + 1])
			value = ft_strjoin_char_to_s1(value, ' ');
		i++;
	}
	free(content);
	return (value);
}

char	*_getenv(t_list *vars, char *name)
{
	char	*value;
	t_var	*env;
	size_t	len;

	value = NULL;
	len = ft_strlen(name);
	while (vars)
	{
		env = vars->content;
		if (!ft_strncmp(env->key, name, len) && ft_strlen(env->key) == len)
			value = env->value;
		vars = vars->next;
	}
	if (!value)
		return (NULL);
	return (_getenv_helper(value));
}

char	*_getpid(void )
{
	char	buf[11];
	ssize_t	bytes;
	int		fd;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: getpid: /proc/self/stat");
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
	buf[bytes] = '\0';
	return (ft_itoa(ft_atoi(buf)));
}
