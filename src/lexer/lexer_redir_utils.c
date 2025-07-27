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
	if (!ft_isprint(input[pos]))
		return (print_syntax_error("newline"));
	else if (input[pos] == '|')
		return (print_syntax_error("|"));
	else if (input[pos] == '>' || input[pos] == '<')
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

static void	traverse_amb_string(char *input, size_t *i)
{
	int	f;

	f = 0;
	while (input[*i])
	{
		if (!f && (is_whitespace(input[*i]) || ft_strchr("|<>", input[*i])))
			break ;
		else if (input[*i] == '"' || input[*i] == '\'')
			f = !f;
		(*i)++;
	}
}

char	*is_ambiguous_redirect(t_shell *shell, char *input, size_t i)
{
	t_lexerctx	ctx;
	char		*word;
	size_t		bkp;

	init_lexerctx(&ctx, shell, input, &i);
	bkp = i;
	word = NULL;
	while (input[i] && !is_whitespace(input[i])
		&& !ft_strchr("|<>", input[i]))
	{
		if (!handle_lexer_word(&ctx, &word))
			return (shell->exit_status = SNTX_EXIT_STATUS, NULL);
		if (word && (ctx.amb == 1 || (ctx.amb == 2 && *word) || \
				(ctx.amb == 3 && *word) || (ctx.amb == 4 && !*word)))
		{
			traverse_amb_string(input, &i);
			return (free(word), ft_substr(input, bkp, i - bkp));
		}
	}
	if ((word && !*word && ctx.amb != 2 && ctx.f)
		|| (ctx.amb == 5 && (!word || (word && !*word))))
		return (free(word), traverse_amb_string(input, &i),
			ft_substr(input, bkp, i - bkp));
	return (free(word), NULL);
}
