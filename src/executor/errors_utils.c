/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 23:23:36 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/18 06:28:08 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	pip_error_exit_child(const char *msg)
{
	perror(msg);
	exit(EXIT_FAILURE);
}

int	export_print_error(char *arg)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

int	execve_print_error(char *arg, int fg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	if (fg == 1)
		ft_putendl_fd(": command not found", STDERR_FILENO);
	else if (fg == 2)
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	else if (fg == 4)
		ft_putendl_fd(": Is a directory", STDERR_FILENO);
	return (1);
}
