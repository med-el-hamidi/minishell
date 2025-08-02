/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <mel-hami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:33:36 by obensarj          #+#    #+#             */
/*   Updated: 2025/08/02 23:30:14 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	put_out(char **args, int i, int new_line)
{
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putchar_fd(' ', 1);
		i++;
	}
	if (new_line)
		ft_putchar_fd('\n', 1);
}

int	builtin_echo(char **args)
{
	int	i;
	int	j;
	int	new_line;

	i = 1;
	new_line = 1;
	if (!args)
		return (1);
	while (args[i] && ft_strncmp(args[i], "-n", 2) == 0)
	{
		j = 2;
		while (args[i][j] == 'n')
			j++;
		if (!args[i][j])
		{
			new_line = 0;
			i++;
		}
		else
			break ;
	}
	put_out(args, i, new_line);
	return (0);
}
