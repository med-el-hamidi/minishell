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
	if (!ft_isprint(input[pos]))
		return (print_syntax_error("newline"));
	else if (!ft_strncmp(input + pos, "||", 2))
		return (print_syntax_error("||"));
	else if (!ft_strncmp(input + pos, "&&", 2))
		return (print_syntax_error("&&"));
	else if (input[pos] == '(')
		return (print_syntax_error("("));
	else if (input[pos] == ')')
		return (print_syntax_error(")"));
	else if (input[pos] == '|')
		return (print_syntax_error("|"));
	else if (!ft_strncmp(&input[pos], ">>", 2))
		return (print_syntax_error(">>"));
	else if (!ft_strncmp(input + pos, "<<<", 3))
		return (print_syntax_error("<<<"));
	else if (!ft_strncmp(input + pos, "<<", 2))
		return (print_syntax_error("<<"));
	else if (input[pos] == '>')
		return (print_syntax_error(">"));
	else if (input[pos] == '<')
		return (print_syntax_error("<"));
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

static void	tr_amb(char *input, size_t *i)
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

char	*is_ambiguous_redirect(t_shell *shell, char *in, size_t i)
{
	t_lexerctx	ctx;
	char		*word;
	size_t		j;

	init_lexerctx(&ctx, shell, in, &i);
	j = i;
	word = NULL;
	while (in[i] && !is_whitespace(in[i]) && !ft_strchr("|<>()", in[i])
		&& ft_strncmp(in + i, "&&", 2))
	{
		if (!handle_lexer_word(&ctx, &word))
			return (shell->exit_status = 2, NULL);
		if (word && (ctx.amb == 1 || (ctx.amb == 2 && *word) || \
				(ctx.amb == 3 && *word) || (ctx.amb == 4 && !*word)))
			return (free(word), traverse_amb_string(in, &i),
				ft_substr(in, j, i - j));
	}
	if ((word && !*word && ctx.amb != 2 && ctx.f)
		|| (ctx.amb == 5 && (!word || (word && !*word))))
		return (free(word), tr_amb(in, &i), ft_substr(in, j, i - j));
	else if (word)
		add_token_word(&ctx, word);
	if (ctx.amb == 1)
		return (free(word), ft_substr(in, j, i - j));
	return (free(word), NULL);
}
