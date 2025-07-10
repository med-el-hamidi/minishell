/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 22:47:28 by mel-hami          #+#    #+#             */
/*   Updated: 2025/07/10 22:47:30 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	skip_whitespace(char *input, size_t *i)
{
	while (is_whitespace(input[*i]))
		(*i)++;
	return (*i);
}

static int	has_whitespace(char *str)
{
	size_t	k;

	if (!str || !*str)
		return (1);
	k = 0;
	while (str[k])
	{
		if (is_whitespace(str[k]))
			return (1);
		k++;
	}
	return (0);
}

char	*is_ambiguous_redirect(t_shell *shell, char *input, size_t i)
{
	size_t	j;
	char	*str;

	while (input[i] && !is_whitespace(input[i])
		&& !ft_strchr("|<>", input[i]))
	{
		if (input[i] == '$' && input[i - 1] != '"' && input[i - 1] != '\''
			&& input[i + 1] != '"' && input[i + 1] != '\'')
		{
			j = i;
			str = accumulate_dollar(shell, input, &j);
			if (str && (!*str || has_whitespace(str)))
			{
				free(str);
				return (ft_substr(input, i, j - 2));
			}
			free(str);
			i = j;
		}
		else
			i++;
	}
	return (NULL);
}
