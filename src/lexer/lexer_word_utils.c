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
	free(tmp);
	free(s2);
	return (s1);
}

char	*ft_strjoin_char_to_s1(char *s1, char c)
{
	const char	arr[2] = {c, 0};
	char		*tmp;

	tmp = s1;
	s1 = ft_strjoin(s1, arr);
	free(tmp);
	return (s1);
}

char	*gethome(t_list	*vars)
{
	char	*home;
	char	*tmp;

	tmp = _getenv(vars, "HOME");
	if (tmp)
		home = ft_strdup(tmp);
	else
		home = ft_strdup("");
	return (home);
}
