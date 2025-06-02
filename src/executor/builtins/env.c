#include "../../../includes/minishell.h"

int	builtin_env(t_list *env_list)
{
	t_list	*ptr;

	ptr = env_list;
	while (ptr)
	{
		if (ptr->content)
			printf("%s=%s\n", ((t_env *)ptr->content)->key, ((t_env *)ptr->content)->value);
		ptr = ptr->next;
	}
	return (0);
}
