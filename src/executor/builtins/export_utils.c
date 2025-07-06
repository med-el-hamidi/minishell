#include "../../../includes/minishell.h"

void	update_shell_var(t_list *node, char *value, t_var_type flag)
{
	t_var	*var;

	if (!node)
		return ;
	var = (t_var *)node->content;
	if (var->value)
		free(var->value);
	if (value)
		var->value = ft_strdup(value);
	else
		var->value = NULL;
	var->flag = flag;
}

void	create_shell_var(t_list **vars, char *key, char *value, t_var_type flag)
{
	t_var	*node;

	node = malloc(sizeof(t_var));
	if (!node)
		return ;
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->flag = flag;
	ft_lstadd_back(vars, ft_lstnew(node));
}

static int	_is_exist(char **keys, char *key)
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
			if (min_var->value)
				printf("declare -x %s=\"%s\"\n", min_var->key, expand_env(vars, min_var->key));
			else
				printf("declare -x %s\n", min_var->key);
			min_keys[i++] = ft_strdup(min_var->key);
			min_keys[i] = NULL;
		}
	}
	free_2d_array(min_keys);
	return (0);
}