/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 19:17:22 by mel-hami          #+#    #+#             */
/*   Updated: 2025/07/13 19:17:23 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static size_t	handle_word_start(char **content, char **word,
	t_lexerctx *ctx, char *tmp)
{
	size_t	j;
	size_t	len;

	len = ft_strlen(tmp);
	j = 0;
	if (*word && *word[0] && content[j])
	{
		*word = ft_strjoin_to_s1(*word, ft_strdup(content[j++]));
		add_token(ctx->tokens, create_token(TOKEN_WORD, *word));
		free(*word);
		*word = NULL;
		if (!content[j] && len && tmp[len - 1] == ' ' && !ctx->amb)
			ctx->amb = 2;
	}
	return (j);
}

static int	handle_dollar_split(char **content, char *tmp,
	t_lexerctx *ctx, char **word)
{
	size_t	j;
	size_t	len;

	j = handle_word_start(content, word, ctx, tmp);
	while (content[j] && content[j + 1])
		add_token(ctx->tokens, create_token(TOKEN_WORD, content[j++]));
	if (content[j])
	{
		if (j > 0)
			ctx->amb = 1;
		len = ft_strlen(tmp);
		if (len && tmp[len - 1] == ' ')
		{
			add_token(ctx->tokens, create_token(TOKEN_WORD, content[j++]));
			if (!ctx->amb)
				ctx->amb = 2;
		}
		if (content[j])
			*word = ft_strdup(content[j]);
		else
			*word = ft_strdup("");
	}
	return (1);
}

static int	handle_dollar(t_lexerctx *ctx, char **word)
{
	char	*tmp;
	char	**content;

	tmp = accu_dollar(ctx->shell, ctx->input, ctx->i, _getenv_al);
	if (!has_whitespace(tmp))
	{
		*word = ft_strjoin_to_s1(*word, tmp);
		return (1);
	}
	content = ft_split_set(tmp, " \t");
	if (!content)
	{
		(free(tmp), free(*word));
		return (ft_lstclear(ctx->tokens, del_token), 0);
	}
	if (*word && *word[0] && is_whitespace(tmp[0]))
	{
		ctx->amb = 1;
		add_token(ctx->tokens, create_token(TOKEN_WORD, *word));
		free(*word);
		*word = NULL;
	}
	if (!handle_dollar_split(content, tmp, ctx, word))
		return (free(tmp), free_2d_array(content), 0);
	return (free(tmp), free_2d_array(content), 1);
}

static char	*handle_tilde(t_shell *shell, size_t *i, char *word, int *f)
{
	*f &= 1;
	(*i)++;
	return (ft_strjoin_to_s1(word, gethome(shell->vars)));
}

int	handle_lexer_loop(t_lexerctx *ctx, char **word, int *f)
{
	char	*tmp;

	if (ctx->input[*ctx->i] == '$')
	{
		*f &= 1;
		if (!handle_dollar(ctx, word))
			return (0);
	}
	else if (((!*ctx->i && ctx->input[*ctx->i] == '~') || (*ctx->i > 0 && \
		is_whitespace(ctx->input[*ctx->i - 1]) && ctx->input[*ctx->i] == '~')) \
		&& (is_whitespace(ctx->input[*ctx->i + 1]) || \
		!ctx->input[*ctx->i + 1] || ctx->input[*ctx->i + 1] == '/'))
		*word = handle_tilde(ctx->shell, ctx->i, *word, f);
	else
	{
		*f &= 0;
		tmp = accumulate_token(ctx->shell, ctx->input, ctx->i);
		if (!tmp && *word)
			return (free(*word), *word = NULL, (*f &= 3), 0);
		*word = ft_strjoin_to_s1(*word, tmp);
		if (!*word)
			return ((*f &= 3), 0);
	}
	return (1);
}
