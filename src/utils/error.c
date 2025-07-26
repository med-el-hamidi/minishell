/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:07:01 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:07:02 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	_print_error(char *msg, const int sys_error)
{
	ft_putstr_fd("minishell: error: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	if (sys_error)
	{
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
	}
	ft_putchar_fd('\n', STDERR_FILENO);
}

int	print_syntax_error(const char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", \
														STDERR_FILENO);
	ft_putstr_fd((char *)token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	return (SNTX_EXIT_STATUS);
}

void	open_script_err_exit(char *script, char *msg, int e)
{
	ft_putstr_fd(script, STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
	exit(e);
}
