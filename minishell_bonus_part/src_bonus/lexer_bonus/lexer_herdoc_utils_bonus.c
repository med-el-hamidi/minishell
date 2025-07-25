/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_herdoc_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 18:46:35 by mel-hami          #+#    #+#             */
/*   Updated: 2025/07/13 18:46:36 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

int	should_parse_dollar(char *input, size_t i)
{
	while (input[i] && !ft_strchr("|<>", input[i])
		&& ft_strncmp(input + i, "&&", 2))
	{
		if ((input[i] == '"' || input[i] == '\''))
			return (0);
		i++;
	}
	return (1);
}

char	*get_delimiter(char *input, size_t	*i, int f)
{
	char	*result;

	result = NULL;
	while (input[*i])
	{
		if (!f && (is_whitespace(input[*i]) || ft_strchr("|<>", input[*i])
			|| !ft_strncmp(input + *i, "&&", 2)))
			break ;
		else if (input[*i] == '"' || input[*i] == '\'')
		{
			if (!f && check_unclosed_quotes(input, *i))
				return (free(result), NULL);
			if (!f)
				f = 1;
			else
				f = 0;
			(*i)++;
		}
		else if (input[*i] == '$'
			&& (input[*i + 1] == '"' || input[*i + 1] == '\''))
			(*i)++;
		else
			result = ft_strjoin_char_to_s1(result, input[(*i)++]);
	}
	return (result);
}
