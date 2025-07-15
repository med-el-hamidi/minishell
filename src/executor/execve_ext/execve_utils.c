/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 02:01:55 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/15 14:45:00 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	is_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0)
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

char	*get_cmd_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*full_path;
	int		i;
	char	*env;

	env = expand_env(shell->vars, "PATH");
	i = 0;
	if (!cmd || ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	if (!env)
	{
		if (is_directory(cmd))
			return (NULL);
		return (execv_print_error(cmd, 2), NULL);
	}
	paths = ft_split(env, ':');
	while (paths && paths[i])
	{
		full_path = join_3(paths[i], "/", cmd);
		if (access(full_path, X_OK) == 0)
			return (free_2d_array(paths), full_path);
		free(full_path);
		i++;
	}
	free_2d_array(paths);
	return (execv_print_error(cmd, 1), NULL);
}

static int	_envp_helper(t_var *env, char **envp, int *i)
{
	if (!env || !env->key)
		return (1);
	envp[*i] = join_3(env->key, "=", env->value);
	if (!envp[(*i)++])
		return (free_2d_array(envp), 1);
	return (0);
}

char	**env_list_to_envp(t_list *vars)
{
	t_list	*ptr;
	t_var	*env;
	int		i;
	char	**envp;

	i = 0;
	envp = malloc(sizeof(char *) * (ft_lstsize(vars) + 1));
	if (!envp)
		return (perror("failed to malloc env_list_to_envp"), NULL);
	ptr = vars;
	while (ptr)
	{
		env = (t_var *)ptr->content;
		if (env->flag == VAR_ENV && _envp_helper(env, envp, &i))
			return (NULL);
		ptr = ptr->next;
	}
	envp[i] = NULL;
	return (envp);
}
