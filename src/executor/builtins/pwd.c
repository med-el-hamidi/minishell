/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:28:42 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/07 22:28:43 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	builtin_pwd(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
	{
		perror("pwd");
		return (1);
	}
	ft_putendl_fd(buf, STDOUT_FILENO);
	free(buf);
	return (0);
}
