/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:28:33 by obensarj          #+#    #+#             */
/*   Updated: 2025/08/02 11:47:25 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	traverse_vars(t_list **vars, char *arg)
{
	t_list	*current;
	t_list	*prev;

	current = *vars;
	prev = NULL;
	while (current)
	{
		if (!ft_strcmp(((t_var *)current->content)->key, arg))
		{
			unset_node(vars, &current, &prev);
			break ;
		}
		update_iterators(&prev, &current);
	}
}

int	builtin_unset(char **args, t_list **vars)
{
	char	**invalid_args;
	int		i;
	int		invalid_count;

	if (!args || !*vars)
		return (0);
	invalid_args = malloc(sizeof(char *) * count_2d_array(args));
	if (!invalid_args)
		return (perror("malloc unset failed"), 1);
	invalid_count = 0;
	i = 0;
	while (args[++i])
	{
		if (!is_valid_identifier(args[i]))
			invalid_args[invalid_count++] = args[i];
		else
			traverse_vars(vars, args[i]);
	}
	i = 0;
	while (i < invalid_count)
		export_print_error(invalid_args[i++]);
	free(invalid_args);
	return (invalid_count > 0);
}

void	unset_node(t_list **vars, t_list **current, t_list **prev)
{
	t_list	*to_delet;

	to_delet = *current;
	if (!*prev)
		*vars = (*current)->next;
	else
		(*prev)->next = (*current)->next;
	*current = (*current)->next;
	ft_lstdelone(to_delet, del_env);
}

void	update_iterators(t_list **prev, t_list **current)
{
	*prev = *current;
	*current = (*current)->next;
}
