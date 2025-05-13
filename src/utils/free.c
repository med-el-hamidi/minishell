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
