#include "../../../includes/minishell.h"

static int print_error(char *arg)
{
	ft_putstr_fd("minishell: export: '", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
    return (1);
}

static int is_valid_identifier(const char *arg)
{
    int i;

	i = 0;
    if (!arg || (!ft_isalpha(arg[i]) && arg[i] != '_'))
        return (0);
	else
		i++;
    while (arg[i] && arg[i] != '=')
    {
		if (!ft_isalnum(arg[i]) && arg[i] != '_')
			return (0);
        i++;
    }
    return (1);
}

static int	add_to_env(char *arg, t_list **vars)
{
	t_list	*existing;
	t_var	*add_env_var_node;
	char	*equal_pos;
	char	*value;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return (1);//return (add_to_exported_list(arg));
	*equal_pos = '\0';
	value = equal_pos + 1;
	existing = find_env_var(*vars, arg);
	if (existing)
	{
		free(((t_var *)existing->content)->value);
		((t_var *)existing->content)->value = ft_strdup(value);
	}
	else
	{
		add_env_var_node = malloc(sizeof(t_var));
		if (!add_env_var_node)
			return (errno = ENOMEM, perror("minishell: malloc export add_node"), 1);
		add_env_var_node->key = ft_strdup(arg);
		add_env_var_node->value = ft_strdup(value);
		ft_lstadd_back(vars, ft_lstnew(add_env_var_node));
	}
	return (0);
}

static int main_export(char **arg, t_list **vars)
{
    int i;
    int j;
    char **invalid_args;
    int invalid_count;
    int arg_counter;

	invalid_count= 0;
	arg_counter = ft_argv_count(arg);
    invalid_args = malloc(sizeof(char *) * arg_counter);
	i = 0;
    if (!invalid_args)
    	return ((errno = ENOMEM), perror("malloc export failed"), 1);
    while ( arg[++i])
    {
        if (!is_valid_identifier(arg[i]))
            invalid_args[invalid_count++] = arg[i];
        else
			add_to_env(arg[i], vars);
    }
	j = -1;
    while (++j < invalid_count)
        print_error(invalid_args[j]);
    free(invalid_args);
    return (invalid_count > 0);
}

int	builtin_export(char **argv, t_list **vars)
{
	if (!argv || !vars)
		return (1);
	if (!argv[1])
		return (_export(*vars));
	if (main_export(argv, vars))
		return (1);
	return (0);
}
