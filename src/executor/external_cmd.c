#include "../../includes/minishell.h"

char **env_list_to_envp(t_list *vars)
{
	t_list *ptr;
	t_var	*env;
	int		i;
	int		len;
	char	**envp;

	i = 0;
	len  = ft_lstsize(vars);
	envp = malloc(sizeof(char *) * (len + 1));
	if (!envp)
		return (NULL);
	ptr = vars;
	while (ptr)
	{
		env = (t_var *)ptr->content;
		if (env)
		{
			if (!(envp[i++] = join_3(env->key, "=", env->value)))
			{
				free_2d_array(envp);
				return (NULL);
			}
		}
		else if (env->key)
		{
			if (!(envp[i++] = ft_strdup(env->key)))
			{
				free_2d_array(envp);
				return (NULL);
			}
		}
		ptr = ptr->next;
	}
	envp[i] = NULL;
	return (envp);
}

char *join_3(const char *s1, char *s2, const char *s3)
{
	char	*res;
	char	*tmp;

	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

char    *get_cmd_path(char *cmd, t_shell *shell)
{
    char    **paths;
    char    *full_path;
    int        i;
    char    *env;

	env	= expand_env(shell->vars, "PATH");
    i = 0;
	if (!cmd || ft_strchr(cmd, '/'))
        return (ft_strdup(cmd));
    if (!env)
        return (NULL);

    paths = ft_split(env, ':');
    while (paths && paths[i])
    {
        full_path = join_3(paths[i], "/", cmd);
        if (access(full_path, X_OK) == 0)
        {
            free_2d_array(paths);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    free_2d_array(paths);
    return (NULL);
}

int	exec_external(t_ast *node, t_shell *shell)
{
	pid_t	pid;
	int		status;
	char	*path;
	char	**envp;

	path = get_cmd_path(node->args[0], shell);
	if (!path)//check
	{
		perror("command not found");
		return (127);
	}
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
	free(path);
	free_2d_array(envp);
	return (shell->exit_status);
}
