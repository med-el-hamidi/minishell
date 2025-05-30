#include "../../../includes/minishell.h"

int	builtin_unset(char **argv, t_list **env_list)
{
	t_list	*current;
	t_list	*prev;
	int		i;

	if (!argv || !env_list)
		return (0);
	i = 1;
	current = *env_list;
	prev = NULL;
	while (current)
	{
		if (!ft_strcmp(((t_env *)current->content)->key, argv[i]))
		{
			if (!prev)
				*env_list = current->next;
			else
				prev->next = current->next;
			ft_lstdelone(current, del_env);
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
