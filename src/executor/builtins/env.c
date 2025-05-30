#include "../../../includes/minishell.h"

int	builtin_env(t_llist *env_list)
{
	while (env_list)
	{
		if (env_list->value)
			printf("%s=%s\n", ((t_env_v *)env_list->value)->key, ((t_env_v *)env_list->value)->val);
		env_list = env_list ->next;
	}
	return (0);
}

t_env	*env_new_node(char *name, char *value)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = name;
	node->value = value;
	node->next = NULL;
	return (node);
}

t_env	*init_env(char **envp)
{
	int		i;
	t_env	*head;
	t_env	*node;
	char	**env_v;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		env_v = ft_split(envp[i], '=');
		node = env_new_node(env_v[0], env_v[1]);		
		if (!node)
		{

			return (head);
		}
		if (!head)
			head = node;
		else
			head->next = node;
		i++;
	}	
	return (head);
}
