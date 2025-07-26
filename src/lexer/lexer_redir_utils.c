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
	if (!str || !*str)
		return (0);
	while (*str)
	{
		if (is_whitespace(*str))
			return (1);
		str++;
	}
	return (0);
}

char	*is_ambiguous_redirect(t_shell *shell, char *input, size_t i)
{
	t_lexerctx	ctx;
	char		*word;
	size_t		bkp;
	int			f;

	ctx.shell = shell;
	ctx.tokens = NULL;
	ctx.input = input;
	ctx.i = &i;
	bkp = i;
	f = 1;
	word = NULL;
	ctx.amb = 0;
	while (input[i] && !is_whitespace(input[i])
		&& !ft_strchr("|<>", input[i]))
	{
		if (!handle_lexer_word(&ctx, &word, &f))
			return (shell->exit_status = SNTX_EXIT_STATUS, NULL);
		if (word && (ctx.amb == 1 || (ctx.amb == 2 && *word) || \
				(ctx.amb == 3 && *word) || (ctx.amb == 4 && !*word)))
			return (free(word), ft_substr(input, bkp, i - bkp));
	}
	if ((word && !*word && ctx.amb != 2 && f) || (ctx.amb < 2 && !word))
		return (free(word), ft_substr(input, bkp, i - bkp));
	return (free(word), NULL);
}
