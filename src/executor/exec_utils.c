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

t_list	*find_env_var(t_list *vars, char *name)
{
	t_list	*ptr;

	ptr = vars;
	while (ptr)
	{
		if (!(ft_strcmp(((t_var *)ptr->content)->key, name)))
			return (ptr);
		ptr = ptr->next;
	}
	return (NULL);
}

int	ft_strisnum(char *s)
{
	if (!s || !*s)
		return (0);
	if (*s == '+' || *s == '-')
		s++;
	if (!ft_isdigit(*s))
		return (0);
	while (*s)
	{
		if (ft_isdigit(*s) == 0)
			return (0);
		s++;
	}
	return (1);	
}

/* parser utils removed after merging */

char	*expand_env(t_list *vars, const char *name)
{
	t_list	*ptr;
	t_var	*env;
    size_t	len;

	len = ft_strlen(name);
	ptr = vars;
    while (ptr)
    {
        env = ptr->content;
        if (!ft_strncmp(env->key, name, len) && ft_strlen(env->key) == len)
            return (env->value);
		ptr = ptr->next;
    }
    return (NULL);
}