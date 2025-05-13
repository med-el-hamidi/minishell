# include "../../includes/minishell.h"

void	free_2d_array(char **arr)
{
	if (!arr)
		return ;
	while (*arr)
		free(*arr++);
}
