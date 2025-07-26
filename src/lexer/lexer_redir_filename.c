/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_redir_filename.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/25 14:20:37 by mel-hami          #+#    #+#             */
/*   Updated: 2025/07/25 14:20:39 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	redir_filename_loop(t_lexerctx *ctx, char *input,
	char **result, int *f)
{
	while (input[*ctx->i] && !is_whitespace(input[*ctx->i])
		&& !ft_strchr("|<>", input[*ctx->i]))
	{
		if (!handle_lexer_word(ctx, result, f))
		{
			free(*result);
			ft_lstclear(ctx->tokens, del_token);
			return (0);
		}
	}
	return (1);
}

static char	*join_redir_filename_tokens(t_list *tokens)
{
	char	*result;
	t_list	*ptr;
	t_token	*t;

	result = NULL;
	ptr = tokens;
	if (ptr)
	{
		t = (t_token *)ptr->content;
		if (t && t->value)
			result = ft_strjoin_to_s1(result, ft_strdup(t->value));
	}
	return (result);
}

char	*get_redir_filename(t_shell *shell, char *input, size_t *i)
{
	t_lexerctx	ctx;
	t_list		*tokens;
	char		*result;
	int			f;

	tokens = NULL;
	ctx.shell = shell;
	ctx.tokens = &tokens;
	ctx.input = input;
	ctx.i = i;
	f = 1;
	result = NULL;
	if (!redir_filename_loop(&ctx, input, &result, &f))
		return (shell->exit_status = SNTX_EXIT_STATUS, NULL);
	if (result)
	{
		add_token(&tokens, create_token(TOKEN_WORD, result));
		free(result);
	}
	result = join_redir_filename_tokens(tokens);
	ft_lstclear(&tokens, del_token);
	return (result);
}
