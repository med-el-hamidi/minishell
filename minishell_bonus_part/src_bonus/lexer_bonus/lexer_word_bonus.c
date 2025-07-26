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

#include "../../includes_bonus/minishell_bonus.h"

char	*accumulate_word(char *input, size_t *i)
{
	char	*result;

	result = ft_strdup("");
	while (input[*i] && !is_whitespace(input[*i])
		&& !ft_strchr("|<>$\"'", input[*i])
		&& ft_strncmp(input + *i, "&&", 2))
	{
		result = ft_strjoin_char_to_s1(result, input[*i]);
		(*i)++;
	}
	return (result);
}

static char	*_handle_special_pos_param(t_shell *sh, char *s, size_t *i)
{
	char	*str;

	str = NULL;
	if (sh->is_interactive && s[*i] == '0')
		str = ft_strdup(sh->argv[0]);
	else if (!sh->is_interactive && ((s[*i] - '0') + 1) < sh->argc)
		str = ft_strdup(sh->argv[(s[*i] - '0') + 1]);
	(*i)++;
	if (!str)
		return (ft_strdup(""));
	return (str);
}

char	*accumulate_dollar(t_shell *sh, char *s, size_t *i,
	char *f(t_list *, char *))
{
	char	*key;
	char	*val;
	size_t	start;

	if (s[++(*i)] == '?')
		return ((*i)++, ft_itoa(sh->exit_status));
	else if (s[*i] == '$')
		return ((*i)++, ft_strdup(""));
	else if (ft_isdigit(s[*i]))
		return (_handle_special_pos_param(sh, s, i));
	else if (!s[*i] || is_whitespace(s[*i]))
		return (ft_strdup("$"));
	start = *i;
	while (ft_isalnum(s[*i]) || s[*i] == '_')
		(*i)++;
	key = ft_substr(s, start, *i - start);
	val = f(sh->vars, key);
	free(key);
	if (!val)
		return (ft_strdup(""));
	return (val);
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
					accumulate_dollar(shell, input, i, _getenv_al));
		else
			result = ft_strjoin_char_to_s1(result, input[(*i)++]);
	}
	if (input[*i] == quote)
		(*i)++;
	return (result);
}

char	*accumulate_other(t_shell *shell, char *input, size_t *i, int *f)
{
	char	*result;
	char	*chunk;

	result = ft_strdup("");
	while (input[*i] && !is_whitespace(input[*i])
		&& !ft_strchr("|<>$", input[*i])
		&& ft_strncmp(input + *i, "&&", 2))
	{
		chunk = NULL;
		if (input[*i] == '"' || input[*i] == '\'')
		{
			chunk = accumulate_quoted(shell, input, i);
			if (chunk && ft_strchr(chunk, '*'))
				*f |= 7;
		}
		else
			chunk = accumulate_word(input, i);
		if (!chunk)
			return (free(result), NULL);
		result = ft_strjoin_to_s1(result, chunk);
	}
	return (result);
}
