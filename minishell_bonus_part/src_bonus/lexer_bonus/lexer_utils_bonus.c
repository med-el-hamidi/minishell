/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:12:22 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:12:24 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (value)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	return (token);
}

void	add_token(t_list **tokens, t_token *token)
{
	if (!token)
		return ;
	else if (!tokens)
	{
		del_token(token);
		return ;
	}
	ft_lstadd_back(tokens, ft_lstnew(token));
}

int	is_whitespace(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
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

void	add_token_word(t_lexerctx *ctx, char *word)
{
	t_list	*words;
	t_list	*ptr;
	size_t	i;

	if (!word)
		return ;
	if (ctx->f != 7 && ft_strchr(word, '*'))
	{
		words = handle_glob(word);
		if (!words)
			return ;
		i = 0;
		ptr = words;
		while (ptr)
		{
			if (++i == 2)
				ctx->amb = 1;
			add_token(ctx->tokens, create_token(TOKEN_WORD,
					(char *)ptr->content));
			ptr = ptr->next;
		}
		ft_lstclear(&words, free);
		return ;
	}
	add_token(ctx->tokens, create_token(TOKEN_WORD, word));
}
