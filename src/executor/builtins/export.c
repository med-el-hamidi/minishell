#include "../../../includes/minishell.h"

int	builtin_export(char **argv, t_list **env_list)
{
	t_list	*existing;
	t_env	*env_var;
	char	*equal_pos;
	char	*val;
	int		argv_count;

	if (!argv || !env_list)
		return (1);
	if (!argv[1])
		return (1);
	argv_count = ft_argv_count(argv);
	while (--argv_count)
	{
		equal_pos = ft_strchr(argv[argv_count], '=');
		if(!equal_pos)
			return (1);
		
		*equal_pos = '\0';
		val = equal_pos + 1;

		if ((existing = find_env_var(*env_list, argv[argv_count])))
		{
			free(((t_env *)existing->content)->value);
			((t_env *)existing->content)->value = ft_strdup(val);
		}
		else
		{
			env_var = malloc(sizeof(t_env));
			if (!env_var)
				return (1);
			env_var->key = ft_strdup(argv[argv_count]);
			env_var->value = ft_strdup(val);
			ft_lstadd_back(&(*env_list), ft_lstnew(env_var));
		}
	}
	return (0);
}
