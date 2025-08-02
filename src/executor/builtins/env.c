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
