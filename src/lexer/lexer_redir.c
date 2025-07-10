/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:13:40 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:13:41 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	check_invalid_token(char *input, size_t pos)
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

static char	*get_delimiter(char *input, size_t *i)
{
	char	*result;
	int		f;

	f = 0;
	result = ft_strdup("");
	while (input[*i] && !ft_strchr("|<>", input[*i]))
	{
		if (!f && is_whitespace(input[*i]))
			break ;
		else if ((input[*i] == '"' || input[*i] == '\''))
		{
			if (!f && check_unclosed_quotes(input, *i))
				return (free(result), NULL);
			if (!f)
				f = 1;
			else
				f = 0;
			(*i)++;
		}
		else
			result = ft_strjoin_char_to_s1(result, input[(*i)++]);
	}
	return (result);
}

static int	get_redirection_type(char *input, size_t *i)
{
	int	type;

	type = TOKEN_REDIR_IN;
	if (input[*i] == '>' && input[*i + 1] == '>')
		type = TOKEN_REDIR_APPEND;
	else if ((input[*i] == '<' && input[*i + 1] == '<'))
		type = TOKEN_REDIR_HEREDOC;
	else if (input[*i] == '>')
		type = TOKEN_REDIR_OUT;
	if (type == TOKEN_REDIR_APPEND || type == TOKEN_REDIR_HEREDOC)
		*i += 1;
	*i += 1;
	return (type);
}

static char	*_get_redir_value(t_shell *shell, char *input, size_t *i, int type)
{
	char	*str;
	char	*amb;

	if (type == TOKEN_REDIR_HEREDOC)
		str = get_delimiter(input, i);
	else
	{
		amb = is_ambiguous_redirect(shell, input, *i);
		if (!amb)
			str = accumulate_token(shell, input, i);
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(amb, STDERR_FILENO);
			ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
			free(amb);
			return (NULL);
		}
	}
	return (str);
}

int	handle_redirection(t_shell *shell, t_list **tokens, char *input, size_t *i)
{
	int		type;
	char	*str;

	type = get_redirection_type(input, i);
	if (check_invalid_token(input, *i))
		return (2);
	while (is_whitespace(input[*i]))
		(*i)++;
	str = _get_redir_value(shell, input, i, type);
	if (!str)
		return (1);
	if (type == TOKEN_REDIR_APPEND)
		add_token(tokens, create_token(TOKEN_REDIR_APPEND, str));
	else if (type == TOKEN_REDIR_HEREDOC)
		(set_herdoc_tmp_file(shell, &str), \
		add_token(tokens, create_token(TOKEN_REDIR_HEREDOC, str)));
	else if (type == TOKEN_REDIR_OUT)
		add_token(tokens, create_token(TOKEN_REDIR_OUT, str));
	else
		add_token(tokens, create_token(TOKEN_REDIR_IN, str));
	return (free(str), 0);
}
