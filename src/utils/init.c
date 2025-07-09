/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 19:38:36 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 19:38:38 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_var	*create_env_v(char *str)
{
	t_var	*v;

	if (!str)
		return (NULL);
	v = malloc(sizeof(t_var));
	if (!v)
		return (NULL);
	v->key = ft_substr(str, 0, ft_strchr(str, '=') - str);
	if (ft_strchr(str, '=') + 1)
		v->value = ft_strdup(ft_strchr(str, '=') + 1);
	else
		v->value = NULL;
	v->flag = VAR_ENV;
	return (v);
}

t_list	*init_env(char **envp)
{
	t_list	*head_env;
	t_list	*node_env;
	t_var	*v;
	int		i;

	if (!envp || !*envp)
		return (NULL);
	head_env = NULL;
	i = -1;
	while (envp[++i])
	{
		v = create_env_v(envp[i]);
		if (!v)
			return (ft_lstclear(&head_env, del_env), NULL);
		node_env = ft_lstnew(v);
		if (!node_env)
			return (ft_lstclear(&head_env, del_env), NULL);
		ft_lstadd_back(&head_env, node_env);
	}
	if (!getenv("PATH"))
		create_shell_var(&head_env, "PATH", \
"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", VAR_LOCAL);
	increment_shell_level(&head_env);
	return (head_env);
}

static void	_set_history_sizes(t_shell *shell)
{
	char	*val;
	char	*nbr;
	int		n;

	n = 0;
	val = getenv("HISTSIZE");
	if (val)
		n = ft_atoi(val);
	if (n <= 0)
	{
		n = HISTSIZE;
		nbr = ft_itoa(n);
		create_shell_var(&shell->vars, "HISTSIZE", nbr, VAR_LOCAL);
		free(nbr);
	}
	shell->history.histsize = n;
	n = 0;
	val = getenv("HISTFILESIZE");
	if (val)
		n = ft_atoi(val);
	if (n <= 0)
	{
		n = HISTFILESIZE;
		nbr = ft_itoa(n);
		create_shell_var(&shell->vars, "HISTFILESIZE", nbr, VAR_LOCAL);
		free(nbr);
	}
	shell->history.histfilesize = n;
}

void	init_history(t_shell *shell)
{
	shell->history.count = 0;
	shell->history.current = 0;
	shell->history.histmem_lines_c = 0;
	_set_history_sizes(shell);
	shell->history.entries = malloc((shell->history.histfilesize + 1) \
														* sizeof(char *));
	if (!shell->history.entries)
	{
		perror("minishell: history is not initialized!");
		return ;
	}
	ft_bzero(shell->history.entries, (shell->history.histfilesize + 1) * \
															sizeof(char *));
	set_histfile(shell);
	if (!shell->history.path || !*shell->history.path)
		return ;
	if (!access(shell->history.path, F_OK | R_OK))
		load_history(shell);
}

void	init_termios(t_shell *shell)
{
	tcgetattr(STDIN_FILENO, &shell->orig_termios);
	shell->new_termios = shell->orig_termios;
	shell->new_termios.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->new_termios);
}
