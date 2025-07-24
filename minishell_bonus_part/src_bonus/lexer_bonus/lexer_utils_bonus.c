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
	if (!tokens || !token)
		return ;
	ft_lstadd_back(tokens, ft_lstnew(token));
}

int	is_whitespace(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

void	add_token_word(t_list **tokens, char *word)
{
	t_token	*token;

	if (!tokens || !word)
		return ;
	if (ft_strchr(word, '*'))
	{
		handle_glob(tokens, word);
		return ;
	}
	token = create_token(TOKEN_WORD, word);
	if (!token)
		return ;
	ft_lstadd_back(tokens, ft_lstnew(token));
}
