# include "../../includes/minishell.h"

void	free_2d_array(char **arr)
{
	char	**ptr;

	if (!arr)
		return ;
	ptr = arr;
	while (*ptr)
		free(*ptr++);
	free(arr);
}

void	del_env(void *v)
{
	t_env	*ptr;

	if (!v)
		return ;
	ptr = (t_env *)v;
	free(ptr->key);
	free(ptr->value);
	free(ptr);
}
