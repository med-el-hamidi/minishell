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
		if (++lvl > 1999)
		{
			ft_putstr_fd("minishell: warning: shell level (", STDERR_FILENO);
			ft_putnbr_fd(lvl, STDERR_FILENO);
			ft_putstr_fd(") too high, resetting to 1\n", STDERR_FILENO);
			lvl = 1;
		}
		if (lvl < 0)
			lvl = 0;
		str_lvl = ft_itoa(lvl);
		update_shell_var(shlvl, str_lvl, ((t_var *)shlvl->content)->flag);
		free(str_lvl);
	}
}

void	init_shell_vars(char *name, t_list **vars)
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
	if (!getenv("_"))
		create_shell_var(vars, "_", name, VAR_ENV);
	increment_shell_level(vars);
}
