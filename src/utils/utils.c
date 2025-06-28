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
