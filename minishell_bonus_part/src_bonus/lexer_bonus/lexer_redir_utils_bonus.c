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

#include "../../includes_bonus/minishell_bonus.h"

int	check_invalid_token(char *input, size_t pos)
{
	while (is_whitespace(input[pos]))
		pos++;
	if (!ft_isprint(input[pos]))
		return (print_syntax_error("newline"));
	if (input[pos] == '>' || input[pos] == '<')
	{
		if (!ft_strncmp(&input[pos], ">>", 2))
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

static void	init_lexerctx(t_lexerctx *ctx, t_shell *shell, char *input,
																size_t *i)
{
	if (!ctx)
		return ;
	ctx->shell = shell;
	ctx->tokens = NULL;
	ctx->input = input;
	ctx->i = i;
	ctx->f = 1;
	ctx->amb = 0;
}

char	*is_ambiguous_redirect(t_shell *shell, char *input, size_t i)
{
	t_lexerctx	ctx;
	char		*word;
	size_t		bkp;

	init_lexerctx(&ctx, shell, input, &i);
	bkp = i;
	word = NULL;
	while (input[i] && !is_whitespace(input[i]) && !ft_strchr("|<>", input[i])
		&& ft_strncmp(input + i, "&&", 2))
	{
		if (!handle_lexer_word(&ctx, &word))
			return (shell->exit_status = 2, NULL);
		if (word && (ctx.amb == 1 || (ctx.amb == 2 && *word) || \
				(ctx.amb == 3 && *word) || (ctx.amb == 4 && !*word)))
			return (free(word), ft_substr(input, bkp, i));
	}
	if ((word && !*word && ctx.amb != 2 && ctx.f) || (ctx.amb < 2 && !word))
		return (free(word), ft_substr(input, bkp, i));
	else if (word)
		add_token_word(&ctx, word);
	if (ctx.amb == 1)
		return (free(word), ft_substr(input, bkp, i));
	return (free(word), NULL);
}
