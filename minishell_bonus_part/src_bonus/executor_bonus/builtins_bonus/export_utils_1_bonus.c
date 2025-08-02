#include "../../../includes_bonus/minishell_bonus.h"

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
	{
		perror("failed to malloc create_shell_var");
		return ;
	}
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	node->flag = flag;
	ft_lstadd_back(vars, ft_lstnew(node));
}

void	_add_or_update_path_cd(t_list **vars, char *name, char *value)
{
	t_list	*node;

	node = find_shell_var(*vars, name);
	if (node)
		update_shell_var(node, value, VAR_ENV);
	else
		create_shell_var(vars, name, value, VAR_ENV);
}

void	update_lastarg_var(t_list *vars, char *value)
{
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		update_shell_var(find_shell_var(vars, "_"), value, VAR_ENV);
	else
		update_shell_var(find_shell_var(vars, "_"), "", VAR_ENV);
}
