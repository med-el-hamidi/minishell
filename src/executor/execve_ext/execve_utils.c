/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 02:01:55 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/18 06:09:54 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	is_directory(char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat))
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}

int	is_regular_file(char *path)
{
	struct stat	path_stat;

	if (access(path, F_OK) || stat(path, &path_stat))
		return (0);
	return (S_ISREG(path_stat.st_mode));
}

char	*get_cmd_path(char *cmd, t_shell *shell)
{
	char	**paths;
	char	*full_path;
	int		i;
	char	*env;

	if (!cmd)
		return (NULL);
	else if (ft_strchr(cmd, '/') && is_regular_file(cmd))
		return (ft_strdup(cmd));
	else if (cmd[0] == '/' || !strncmp(cmd, "./", 2) || !strncmp(cmd, "../", 3)
		|| (ft_strlen(cmd) > 0 && cmd[ft_strlen(cmd) - 1] == '/'))
		return (NULL);
	env = expand_env(shell->vars, "PATH");
	if (!env)
		return (NULL);
	paths = ft_split(env, ':');
	i = 0;
	while (paths && paths[i])
	{
		full_path = join_3(paths[i++], "/", cmd);
		if (is_regular_file(full_path))
			return (free_2d_array(paths), full_path);
		free(full_path);
	}
	return (free_2d_array(paths), NULL);
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
