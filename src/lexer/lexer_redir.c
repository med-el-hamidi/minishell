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

static char	*get_delimiter_al(char *input, size_t *i)
{
	char	*result;
	int		f;

	f = 0;
	result = NULL;
	while (input[*i] && !ft_strchr("|<>", input[*i]))
	{
		if (!f && is_whitespace(input[*i]))
			break ;
		else if (input[*i] == '"' || input[*i] == '\'')
		{
			if (!f && check_unclosed_quotes(input, *i))
				return (free(result), NULL);
			if (!f)
				f = 1;
			else
				f = 0;
			result = ft_strjoin_char_to_s1(result, input[(*i)++]);
		}
		else
			result = ft_strjoin_char_to_s1(result, input[(*i)++]);
	}
	return (result);
}

char	*get_redir_value(t_shell *shell, char *input, size_t *i, int *type)
{
	char	*str;
	char	*amb;

	str = NULL;
	if (*type == TOKEN_REDIR_HEREDOC)
		str = get_delimiter_al(input, i);
	else
	{
		amb = is_ambiguous_redirect(shell, input, *i);
		if (!amb)
			str = get_redi_file(shell, input, i);
		else
		{
			*type = TOKEN_AMB_REDIR;
			return (amb);
		}
	}
	return (str);
}

static void	add_redir_token(t_list **tokens, char *str, int type)
{
	if (type == TOKEN_REDIR_APPEND)
		add_token(tokens, create_token(TOKEN_REDIR_APPEND, str));
	else if (type == TOKEN_REDIR_OUT)
		add_token(tokens, create_token(TOKEN_REDIR_OUT, str));
	else if (type == TOKEN_REDIR_IN)
		add_token(tokens, create_token(TOKEN_REDIR_IN, str));
	else
		add_token(tokens, create_token(TOKEN_AMB_REDIR, str));
}

int	handle_redirection(t_shell *shell, t_list **tokens, char *input, size_t *i)
{
	char	*str;
	int		type;
	size_t	bkp;

	type = get_redirection_type(input, i);
	if (check_invalid_token(input, *i))
		return (2);
	bkp = skip_whitespace(input, i);
	str = get_redir_value(shell, input, i, &type);
	if (!str && (input[*i] == '\'' || input[*i] == '"'))
		return (2);
	if (type == TOKEN_REDIR_HEREDOC)
	{
		add_token(tokens, create_token(TOKEN_REDIR_HEREDOC, str));
		return (free(str), 0);
	}
	add_redir_token(tokens, str, type);
	return (free(str), 0);
}
