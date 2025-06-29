#include "../../../includes/minishell.h"

int	builtin_env(t_list *env_list)
{
	t_list	*ptr;
	t_env	*var;

	ptr = env_list;
	while (ptr)
	{
		var = ((t_env *)ptr->content);
		if (var->key)
			printf("%s=%s\n", var->key, var->value);
		ptr = ptr->next;
	}
	return (0);
}
