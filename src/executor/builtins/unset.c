#include "../../../includes/minishell.h"

int	builtin_unset(char **argv, t_list **vars)
{
	t_list	*current;
	t_list	*prev;
	int		i;

	if (!argv || !*vars)
		return (0);
	prev = NULL;
	current = *vars;
	while (current)
	{
		i = 0;
		while (argv[++i])
		{
			if (!ft_strcmp(((t_var *)current->content)->key, argv[i]))
			{
				unset_node(vars, &current, &prev);
				break ;
			}
		}
		if (!argv[i])
			update_iterators(&prev, &current);
	}
	return (0);
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
