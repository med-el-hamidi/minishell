/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:23:36 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/12 22:00:37 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	export_print_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

int	execv_print_error(char *arg, int fg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	if (fg == 1)
		ft_putendl_fd(": command not found", STDERR_FILENO);
	if (fg == 2)
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	if (fg == 4)
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
	return (1);
}
