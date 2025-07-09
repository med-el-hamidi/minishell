/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:40:40 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/09 16:22:29 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	print_error(char *arg, int fg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	if (fg == 1)
		ft_putendl_fd(": command not found", STDERR_FILENO);
	if (fg == 2)
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	return (1);
}

static int	_envp_helper(t_var *env, char **envp, int *i)
{
	if (env)
	{
		envp[*i] = join_3(env->key, "=", env->value);
		if (!envp[(*i)++])
			return (free_2d_array(envp), 1);
	}
	else if (env->key)
	{
		envp[*i] = ft_strdup(env->key);
		if (!envp[(*i)++])
			return (free_2d_array(envp), 1);
	}
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
		return (NULL);
	ptr = vars;
	while (ptr)
	{
		env = (t_var *)ptr->content;
		if (_envp_helper(env, envp, &i))
			return (NULL);
		ptr = ptr->next;
	}
	return (envp[i] = NULL, envp);
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
		return (print_error(cmd, 2), NULL);
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
	return (print_error(cmd, 1), NULL);
}

int	exec_external(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**envp;

	path = get_cmd_path(node->args[0], shell);
	if (!path)
		return (127);
	envp = env_list_to_envp(shell->vars);
	pid = fork();
	if (pid == -1)
		return (perror("fork"), 1);
	if (pid == 0)
	{
		execve(path, node->args, envp);
		perror("execve failed");
		exit (126);
	}
	else
	{
		waitpid(pid, &status, 0);
		shell->exit_status = WEXITSTATUS(status);
	}
	return (free(path), free_2d_array(envp), shell->exit_status);
}
