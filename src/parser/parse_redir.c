/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 19:09:18 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 19:09:20 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	_get_redir_type(int type)
{
	if (type == TOKEN_REDIR_IN)
		return (REDIR_INPUT);
	else if (type == TOKEN_REDIR_OUT)
		return (REDIR_OUTPUT);
	else if (type == TOKEN_REDIR_APPEND)
		return (REDIR_APPEND);
	else if (type == TOKEN_REDIR_HEREDOC)
		return (REDIR_HEREDOC);
	return (REDIR_NONE);
}

static char	*get_delimiter(char *input, size_t	i, int f)
{
	char	*result;

	result = NULL;
	while (input[i] && !ft_strchr("|<>", input[i]))
	{
		if (!f && is_whitespace(input[i]))
			break ;
		else if (input[i] == '"' || input[i] == '\'')
		{
			if (!f && check_unclosed_quotes(input, i))
				return (free(result), NULL);
			if (!f)
				f = 1;
			else
				f = 0;
			i++;
			result = ft_strjoin_char_to_s1(result, '\0');
		}
		else if (input[i] == '$'
			&& (input[i + 1] == '"' || input[i + 1] == '\''))
			i++;
		else
			result = ft_strjoin_char_to_s1(result, input[i++]);
	}
	return (result);
}

static int	_should_parse_dollar(char *input)
{
	size_t	i;

	i = 0;
	while (input[i] && !ft_strchr("|<>", input[i]))
	{
		if ((input[i] == '"' || input[i] == '\''))
			return (0);
		i++;
	}
	return (1);
}

static t_ast	*_new_redir_node(t_ast *cmd, t_token *redir_tok, t_shell *shell)
{
	t_ast	*redir_node;
	char	*str;
	int		f;

	redir_node = new_ast_node(AST_REDIR, NULL);
	if (!redir_node)
		return (NULL);
	redir_node->left = cmd;
	redir_node->redir_type = _get_redir_type(redir_tok->type);
	redir_node->redir_fd = -1;
	if (redir_node->redir_type == REDIR_HEREDOC)
	{
		f = _should_parse_dollar(redir_tok->value);
		str = get_delimiter(redir_tok->value, 0, 0);
		shell->exit_status = set_herdoc_tmp_file(shell, &str, f);
		if (shell->exit_status)
			return (free(str), free(redir_node), NULL);
		redir_node->redir_file = str;
	}
	else
		redir_node->redir_file = ft_strdup(redir_tok->value);
	if (!redir_node->redir_file)
		return (free(redir_node), NULL);
	return (redir_node);
}

t_ast	*parse_redirection(t_list **tokens, t_ast *command, t_shell *shell)
{
	t_ast	*redir_node;
	t_token	*redir_tok;

	while (*tokens && is_redirection(((t_token *)(*tokens)->content)->type))
	{
		redir_tok = (t_token *)(*tokens)->content;
		redir_node = _new_redir_node(command, redir_tok, shell);
		if (!redir_node)
			return (free_ast(command), NULL);
		command = redir_node;
		advance_token(tokens);
	}
	return (command);
}
