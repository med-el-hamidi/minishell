/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 18:01:48 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 18:01:50 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*accumulate_word(char *input, size_t *i)
{
	char	*result;

	result = ft_strdup("");
	while (input[*i] && !is_whitespace(input[*i])
		&& !ft_strchr("|<>$\"'", input[*i]))
	{
		result = ft_strjoin_char_to_s1(result, input[*i]);
		(*i)++;
	}
	return (result);
}

char	*accumulate_dollar(t_shell *shell, char *input, size_t *i)
{
	char	*key;
	char	*val;
	size_t	start;

	if (input[++(*i)] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	else if (input[*i] == '$')
	{
		(*i)++;
		return (_getpid());
	}
	else if (!input[*i] || is_whitespace(input[*i]))
		return (ft_strdup("$"));
	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	key = ft_substr(input, start, *i - start);
	val = _getenv(shell->vars, key);
	free(key);
	if (val)
		return (val);
	return (ft_strdup(""));
}

static char	*accumulate_quoted(t_shell *shell, char *input, size_t *i)
{
	char	*result;
	char	quote;

	if (check_unclosed_quotes(input, *i))
		return (NULL);
	result = ft_strdup("");
	quote = input[(*i)++];
	while (input[*i] && input[*i] != quote)
	{
		if (quote == '"' && input[*i] == '$' && input[*i + 1] != '"')
			result = ft_strjoin_to_s1(result, \
								accumulate_dollar(shell, input, i));
		else
			result = ft_strjoin_char_to_s1(result, input[(*i)++]);
	}
	if (input[*i] == quote)
		(*i)++;
	return (result);
}

char	*accumulate_token(t_shell *shell, char *input, size_t *i)
{
	char	*result;
	char	*chunk;

	result = ft_strdup("");
	while (input[*i] && !is_whitespace(input[*i])
		&& !ft_strchr("|<>", input[*i]))
	{
		chunk = NULL;
		if (input[*i] == '"' || input[*i] == '\'')
			chunk = accumulate_quoted(shell, input, i);
		else if (input[*i] == '$')
			chunk = accumulate_dollar(shell, input, i);
		else if (input[*i] == '~' && (!input[*i + 1] || input[*i + 1] == '/'
				|| is_whitespace(input[*i + 1])))
		{
			(*i)++;
			chunk = gethome(shell->vars);
		}
		else
			chunk = accumulate_word(input, i);
		if (!chunk)
			return (free(result), NULL);
		result = ft_strjoin_to_s1(result, chunk);
	}
	return (result);
}
