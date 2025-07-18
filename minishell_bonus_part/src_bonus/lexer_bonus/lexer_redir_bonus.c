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

#include "../../includes_bonus/minishell_bonus.h"

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

char	*get_redir_value(t_shell *shell, char *input, size_t *i, int *type)
{
	char	*str;
	char	*amb;

	str = NULL;
	amb = is_ambiguous_redirect(shell, input, *i);
	if (!amb)
		str = get_redi_file(shell, input, i);
	else
	{
		*type = TOKEN_AMB_REDIR;
		return (amb);
	}
	return (str);
}

static void	add_redir_token(t_list **tokens, char *str, int type)
{
	if (type == TOKEN_REDIR_HEREDOC)
		add_token(tokens, create_token(TOKEN_REDIR_HEREDOC, str));
	else if (type == TOKEN_REDIR_APPEND)
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
	if (type == TOKEN_REDIR_HEREDOC)
	{
		str = get_delimiter(input, i, 0);
		if (!str)
			return (2);
		shell->exit_status = set_herdoc_tmp_file(shell, &str, \
									should_parse_dollar(input, bkp));
		if (shell->exit_status)
			return (unlink(str), free(str), shell->exit_status);
	}
	else
		str = get_redir_value(shell, input, i, &type);
	if (!str)
		return (2);
	add_redir_token(tokens, str, type);
	return (free(str), 0);
}
