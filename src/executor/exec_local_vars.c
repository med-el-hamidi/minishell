#include "../../includes/minishell.h"


int	exec_local_vars(char **args, t_list **vars)
{
	char	*assign;
	char	*value;
	t_var	*var;
	t_list	*node;
	size_t	i;

	i = 0;
	while (args[i])
	{
		assign = ft_strchr(args[i], '=');
		*assign = '\0';
		value = assign + 1;
		node = find_shell_var(*vars, args[i]);
		if (node)
		{
			var = (t_var *)node->content;
			update_shell_var(node, value, VAR_ENV);
		}
		else
			create_shell_var(vars, args[i], value, VAR_LOCAL);
		i++;
	}
	
	return (0);
}
