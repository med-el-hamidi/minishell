/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:36:51 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 16:36:52 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	lexer_word(t_shell *shell, t_list **tokens, char *input, size_t *i)
{
	t_lexerctx	ctx;
	char		*word;
	int			f;

	ctx.shell = shell;
	ctx.tokens = tokens;
	ctx.input = input;
	ctx.i = i;
	f = 1;
	word = NULL;
	while (input[*i] && !is_whitespace(input[*i])
		&& !ft_strchr("|<>", input[*i]))
	{
		if (!handle_lexer_loop(&ctx, &word, &f))
			return (shell->exit_status = 2, ft_lstclear(tokens, del_token), 0);
	}
	if (word && !*word && f)
		return (free(word), 1);
	else if (word)
		add_token(tokens, create_token(TOKEN_WORD, word));
	else if (f == 3)
		return (shell->exit_status = 2, \
						ft_lstclear(tokens, del_token), 0);
	return (free(word), 1);
}

static int	lexer_redir(t_shell	*shell, t_list **tokens, char *input, size_t *i)
{
	shell->exit_status = handle_redirection(shell, tokens, input, i);
	if (shell->exit_status)
	{
		ft_lstclear(tokens, del_token);
		return (0);
	}
	return (1);
}

t_list	*lexer(t_shell *shell, char *input)
{
	t_list	*tokens;
	size_t	i;

	if (g_exit_status)
		shell->exit_status = g_exit_status;
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (is_whitespace(input[i]))
			i++;
		else if (input[i] == '|')
		{
			add_token(&tokens, create_token(TOKEN_PIPE, "|"));
			i++;
		}
		else if (input[i] == '<' || input[i] == '>')
		{
			if (!lexer_redir(shell, &tokens, input, &i))
				return (NULL);
		}
		else if (!lexer_word(shell, &tokens, input, &i))
			return (NULL);
	}
	return (tokens);
}
