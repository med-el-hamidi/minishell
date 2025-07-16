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

int	check_invalid_token(char *input, size_t pos)
{
	while (is_whitespace(input[pos]))
		pos++;
	if (!ft_isprint(input[pos]))
		return (print_syntax_error("newline"));
	if (input[pos] == '>' || input[pos] == '<')
	{
		if (!ft_strncmp(&input[pos], ">>>", 3))
			return (print_syntax_error(">>>"));
		else if (!ft_strncmp(&input[pos], ">>", 2))
			return (print_syntax_error(">>"));
		else if (!ft_strncmp(&input[pos], "<<<", 3))
			return (print_syntax_error("<<<"));
		else if (!ft_strncmp(&input[pos], "<<", 2))
			return (print_syntax_error("<<"));
		else if (input[pos] == '>')
			return (print_syntax_error(">"));
		else if (input[pos] == '<')
			return (print_syntax_error("<"));
	}
	return (0);
}

char	*get_redi_file(t_shell *shell, char *input, size_t *i)
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
			chunk = accu_dollar(shell, input, i, _getenv);
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

size_t	skip_whitespace(char *input, size_t *i)
{
	while (is_whitespace(input[*i]))
		(*i)++;
	return (*i);
}

int	has_whitespace(char *str)
{
	size_t	i;

	if (!str || !*str)
		return (0);
	i = 0;
	while (str[i])
	{
		if (is_whitespace(str[i]))
			return (1);
		i++;
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
			str = accu_dollar(shell, input, &j, _getenv);
			if (str && (!*str || has_whitespace(str)))
				return (free(str), ft_substr(input, i, j - 3));
			free(str);
			i = j;
		}
		else
			i++;
	}
	return (NULL);
}
