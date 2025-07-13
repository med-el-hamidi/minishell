/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 18:01:54 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 18:01:55 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_unclosed_quotes(char *input, size_t i)
{
	if (input[i++] == '\'')
	{
		while (input[i] && input[i] != '\'')
			i++;
		if (input[i] == '\'')
			return (0);
	}
	else
	{
		while (input[i] && input[i] != '"')
			i++;
		if (input[i] == '"')
			return (0);
	}
	ft_putstr_fd("minishell: syntax error: unclosed quotes!\n", STDERR_FILENO);
	return (2);
}

char	*ft_strjoin_to_s1(char *s1, char *s2)
{
	char	*tmp;

	tmp = s1;
	s1 = ft_strjoin(s1, s2);
	if (tmp)
		free(tmp);
	if (s2)
		free(s2);
	return (s1);
}

char	*ft_strjoin_char_to_s1(char *s1, char c)
{
	const char	arr[2] = {c, 0};
	char		*tmp;

	tmp = s1;
	s1 = ft_strjoin(s1, arr);
	if (tmp)
		free(tmp);
	return (s1);
}

char	*_getenv_al(t_list *vars, char *name)
{
	t_var	*env;
	size_t	len;

	len = ft_strlen(name);
	while (vars)
	{
		env = vars->content;
		if (!ft_strncmp(env->key, name, len) && ft_strlen(env->key) == len)
		{
			if (!env->value)
				break ;
			return (ft_strdup(env->value));
		}
		vars = vars->next;
	}
	return (NULL);
}

char	*gethome(t_list	*vars)
{
	char	*home;

	home = _getenv(vars, "HOME");
	if (!home)
		home = ft_strdup("");
	return (home);
}
