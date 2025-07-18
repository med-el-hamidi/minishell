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

static int	check_buffer_binary(unsigned char *buffer, ssize_t size)
{
	int		non_text_count;
	ssize_t	i;

	non_text_count = 0;
	i = 0;
	while (i < size)
	{
		if (!buffer[i])
			return (1);
		if (!(ft_isprint(buffer[i]) || is_whitespace(buffer[i])))
			non_text_count++;
		i++;
	}
	if ((double)non_text_count / size > 0.3)
		return (1);
	return (0);
}

static int	is_binary_file(const char *path, mode_t mode)
{
	int				fd;
	ssize_t			bytes_read;
	unsigned char	buffer[CHUNK_SIZE];
	int				is_binary;

	is_binary = 0;
	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (-1);
	bytes_read = read(fd, buffer, sizeof(buffer));
	if (bytes_read <= 0)
		return (close(fd), -1);
	if (S_ISREG(mode))
	{
		while (!is_binary)
		{
			is_binary = check_buffer_binary(buffer, bytes_read);
			bytes_read = read(fd, buffer, sizeof(buffer));
			if (bytes_read <= 0)
				return (close(fd), -1);
		}
	}
	else if (!isatty(fd))
		is_binary = check_buffer_binary(buffer, bytes_read);
	return (close(fd), is_binary);
}

int	open_script(char *script)
{
	int			fd;
	struct stat	state;

	fd = open(script, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (script && !*script)
			perror(" ");
		else
			perror(script);
		if (!access(script, F_OK))
			exit(126);
		exit(127);
	}
	if (fstat(fd, &state) == -1)
		(close(fd), perror(script), exit(errno));
	else if (is_binary_file(script, state.st_mode) == 1)
	{
		close(fd);
		ft_putstr_fd(script, STDERR_FILENO);
		t_putstr_fd(": cannot execute binary file\n", STDERR_FILENO);
		exit(126);
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
