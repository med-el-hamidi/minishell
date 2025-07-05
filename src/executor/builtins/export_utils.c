#include "../../../includes/minishell.h"

int	_is_exist(char **keys, char *key)
{
	int	i;

	if (!keys)
		return (0);
	i = 0;
	while (keys[i])
	{
		if (!ft_strcmp(keys[i], key))
			return (1);
		i++;
	}
	return (0);
}

int	_export(t_list *vars)
{
	t_var	*var;
	t_var	*min_var;
	t_var	*max_var;
	char	**min_keys;
	t_list	*ptr;
	int		len;
	int		i;

	len = ft_lstsize(vars);
	min_keys = malloc(sizeof(char *) * (len + 1));
	if (!min_keys)
		return (1);
	min_keys[0] = NULL;
	//
	max_var = (t_var *)vars->content;
	ptr = vars;
	while (ptr)
	{
		var = (t_var *)ptr->content;
		if (ft_strcmp(max_var->key, var->key) < 0)
			max_var = var;
		ptr = ptr->next;
	}
	//
	i = 0;
	while (len--)
	{
		min_var = max_var;
		ptr = vars;
		while (ptr)
		{
			var = (t_var *)ptr->content;
			if (!_is_exist(min_keys, var->key) && ft_strcmp(min_var->key, var->key) > 0)
				min_var = var;
			ptr = ptr->next;
		}
		if (min_var && !_is_exist(min_keys, min_var->key) && (min_var->flag == VAR_ENV || min_var->flag == VAR_EXPORTED))
		{
			printf("declare -x %s=\"%s\"\n", min_var->key, expand_env(vars, min_var->key));
			min_keys[i++] = ft_strdup(min_var->key);
			min_keys[i] = NULL;
		}
	}
	free_2d_array(min_keys);
	return (0);
}