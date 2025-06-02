#include "../../../includes/minishell.h"

int	builtin_unset(char **argv, t_list *env_list)
{
	t_list	*current;
	t_list	*to_delet;
	t_list	*prev;
	int		i;
	int		argv_count;

	if (!argv || !env_list)
		return (0);
	argv_count = ft_argv_count(argv) - 1;
	prev = NULL;
	current = env_list;
	while (current)
	{
		i = 0;
		while (argv[++i])
		{
			if (!ft_strcmp(((t_env *)current->content)->key, argv[i]))
			{
				argv_count--;
				to_delet = current;
				
				if (!prev)
				{
					env_list = current->next;
					prev = env_list;
				}
				else
					prev->next = current->next;
				current = prev;
				ft_lstdelone(to_delet, del_env);
				break;
			}
		}
		if (!argv_count)
			break ;
		prev = current;
		current = current->next;
	}
	return (0);
}
