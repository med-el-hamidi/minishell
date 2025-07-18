/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:28:42 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/19 00:14:35 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes_bonus/minishell_bonus.h"

int	builtin_pwd(t_list *vars)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
	{
		printf("%s\n", expand_env(vars, "PWD"));
		return (0);
	}
	ft_putendl_fd(buf, STDOUT_FILENO);
	free(buf);
	return (0);
}
