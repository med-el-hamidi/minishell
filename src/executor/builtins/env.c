#include "../../../includes/minishell.h"

int	builtin_env(t_list *env_list)
{
	while (env_list)
	{
		if (env_list->content)
			printf("%s=%s\n", ((t_env *)env_list->content)->key, ((t_env *)env_list->content)->value);
		env_list = env_list ->next;
	}
	return (0);
}
