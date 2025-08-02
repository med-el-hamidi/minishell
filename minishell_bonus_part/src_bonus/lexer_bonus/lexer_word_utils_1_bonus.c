#include "../../includes_bonus/minishell_bonus.h"

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
		add_token_word(ctx, *word);
		free(*word);
		*word = NULL;
		if (ctx->amb == 2)
			ctx->amb = 4;
		else if (!content[j] && len && is_whitespace(tmp[len - 1])
			&& ctx->amb != 1)
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
		add_token_word(ctx, content[j++]);
	if (content[j])
	{
		if (j > 0)
			ctx->amb = 1;
		len = ft_strlen(tmp);
		if (len && is_whitespace(tmp[len - 1]))
		{
			add_token_word(ctx, content[j++]);
			if (ctx->amb == 2)
				ctx->amb = 4;
			else if (ctx->amb != 1)
				ctx->amb = 2;
		}
		if (content[j])
			*word = ft_strjoin_to_s1(*word, ft_strdup(content[j]));
		else
			*word = ft_strjoin_to_s1(*word, ft_strdup(""));
	}
	return (1);
}

static int	handle_dollar(t_lexerctx *ctx, char **word)
{
	char	*tmp;
	char	**content;

	tmp = accumulate_dollar(ctx->shell, ctx->input, ctx->i, _getenv_al);
	if (!has_whitespace(tmp))
		return (*word = ft_strjoin_to_s1(*word, tmp), 1);
	content = ft_split_set(tmp, " \t");
	if (!content)
		return (free(tmp), free(*word), ft_lstclear(ctx->tokens, del_token), 0);
	if (ctx->amb == 5 && (content[0] || (*word && *word[0])))
		ctx->amb = 0;
	else if (!content[0] && (!*word || (*word && !*word[0])))
		ctx->amb = 5;
	if (*word && *word[0] && is_whitespace(tmp[0]))
	{
		ctx->amb = 3;
		if (content[0])
			ctx->amb = 1;
		(add_token_word(ctx, *word), free(*word));
		*word = NULL;
	}
	if (!handle_dollar_split(content, tmp, ctx, word))
		return (free(tmp), free_2d_array(content), 0);
	return (free(tmp), free_2d_array(content), 1);
}

static char	*handle_tilde(t_shell *shell, char *word, size_t *i, int *f)
{
	if (*f != 7)
		*f &= 1;
	(*i)++;
	return (ft_strjoin_to_s1(word, gethome(shell->vars)));
}

int	handle_lexer_word(t_lexerctx *ctx, char **word)
{
	char	*tmp;

	if (ctx->input[*ctx->i] == '$')
	{
		if (ctx->f != 7)
			ctx->f &= 1;
		if (!handle_dollar(ctx, word))
			return (0);
	}
	else if (((!*ctx->i && ctx->input[*ctx->i] == '~') || (*ctx->i > 0 && \
		is_whitespace(ctx->input[*ctx->i - 1]) && ctx->input[*ctx->i] == '~')) \
		&& (is_whitespace(ctx->input[*ctx->i + 1]) || \
		!ctx->input[*ctx->i + 1] || ctx->input[*ctx->i + 1] == '/'))
		*word = handle_tilde(ctx->shell, *word, ctx->i, &ctx->f);
	else
	{
		ctx->f &= 0;
		tmp = accumulate_other(ctx->shell, ctx->input, ctx->i, &ctx->f);
		if (!tmp && *word)
			return (free(*word), *word = NULL, 0);
		*word = ft_strjoin_to_s1(*word, tmp);
		if (!*word)
			return (0);
	}
	return (1);
}
