#include "../../../includes_bonus/minishell_bonus.h"

static void	traverse_vars(t_list **vars, char *arg)
{
	t_list	*current;
	t_list	*prev;

	if (!ft_strcmp(arg, "_"))
		return ;
	current = *vars;
	prev = NULL;
	while (current)
	{
		if (!ft_strcmp(((t_var *)current->content)->key, arg))
		{
			unset_node(vars, &current, &prev);
			break ;
		}
		prev = current;
		current = current->next;
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
