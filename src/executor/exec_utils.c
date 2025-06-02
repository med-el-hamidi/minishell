#include "../../includes/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while ((s1[i] || s2[i]))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

int ft_argv_count(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

t_list	*find_env_var(t_list *env_list, char *name)
{
	t_list	*ptr;

	ptr = env_list;
	while (ptr)
	{
		if (!(ft_strcmp(((t_env *)ptr->content)->key, name)))
			return (ptr);
		ptr = ptr->next;
	}
	return (NULL);
}