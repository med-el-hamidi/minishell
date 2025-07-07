/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:33:21 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/07 22:33:22 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	builtin_env(t_list *vars)
{
	t_list	*ptr;
	t_var	*var;

	ptr = vars;
	while (ptr)
	{
		var = ((t_var *)ptr->content);
		if (var && var->flag == VAR_ENV)
			printf("%s=%s\n", var->key, var->value);
		ptr = ptr->next;
	}
	return (0);
}
