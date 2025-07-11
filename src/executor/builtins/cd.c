/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 22:33:49 by obensarj          #+#    #+#             */
/*   Updated: 2025/07/10 15:53:43 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static int	cd_prepar_path(t_list *vars, char **arg, char **path)
{
	if (!arg[1])
	{
		*path = expand_env(vars, "HOME");
		if (!*path)
			return (ft_putendl_fd("minishell: cd: HOME not set", \
			STDERR_FILENO), 1);
	}
	else if (arg[1] && arg[2])
		return (3);
	else if (arg[1][0] == '\0' || !ft_strcmp(arg[1], "."))
		return (0);
	else
		*path = arg[1];
	return (4);
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	*path;
	char	*pwd_oldpwd[2];
	int		ret;

	path = NULL;
	ret = cd_prepar_path(shell->vars, argv, &path);
	if (ret == 3)
		return (ft_putendl_fd(CD_ARGS, STDERR_FILENO), 1);
	if (ret == 1)
		return (1);
	if (!ret)
		return (0);
	pwd_oldpwd[1] = getcwd(NULL, 0);
	if (pwd_oldpwd[1])
		update_shell_var(find_shell_var(shell->vars, "OLDPWD"), \
			pwd_oldpwd[1], VAR_ENV);
	else
		perror(CD_RETRIV);
	if (chdir(path) == -1)
		return (free(pwd_oldpwd[1]), perror(path), 1);
	pwd_oldpwd[0] = getcwd(NULL, 0);
	if (pwd_oldpwd[0])
		update_shell_var(find_shell_var(shell->vars, "PWD"), \
			pwd_oldpwd[0], VAR_ENV);
	return (free(pwd_oldpwd[0]), free(pwd_oldpwd[1]), 0);
}
