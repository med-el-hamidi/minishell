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

char	*accumulate_word(char *input, size_t *i)
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

char	*accu_dollar(t_shell *sh, char *s, size_t *i, char *f(t_list *, char *))
{
	char	*key;
	char	*val;
	size_t	start;

	if (s[++(*i)] == '?')
		return ((*i)++, ft_itoa(sh->exit_status));
	else if (s[*i] == '$')
		return ((*i)++, _getpid());
	else if (!s[*i] || is_whitespace(s[*i]))
		return (ft_strdup("$"));
	start = *i;
	while (ft_isalnum(s[*i]) || s[*i] == '_')
		(*i)++;
	key = ft_substr(s, start, *i - start);
	val = f(sh->vars, key);
	free(key);
	if (val)
		return (val);
	return (ft_strdup(""));
}

char	*accumulate_quoted(t_shell *shell, char *input, size_t *i)
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
					accu_dollar(shell, input, i, _getenv_al));
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
		&& !ft_strchr("|<>$", input[*i]))
	{
		chunk = NULL;
		if (input[*i] == '"' || input[*i] == '\'')
			chunk = accumulate_quoted(shell, input, i);
		else
			chunk = accumulate_word(input, i);
		if (!chunk)
			return (free(result), NULL);
		result = ft_strjoin_to_s1(result, chunk);
	}
	return (result);
}
