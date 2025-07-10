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

#include "../../includes/minishell.h"

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

static int	has_whitespace(char *str)
{
	size_t	k;

	if (!str || !*str)
		return (1);
	k = 0;
	while (str[k])
	{
		if (is_whitespace(str[k]))
			return (1);
		k++;
	}
	return (0);
}

char	*is_ambiguous_redirect(t_shell *shell, char *input, size_t i)
{
	size_t	j;
	char	*str;

	while (input[i] && !is_whitespace(input[i])
		&& !ft_strchr("|<>", input[i]))
	{
		if (input[i] == '$')
		{
			j = i;
			str = accumulate_dollar(shell, input, &j);
			if (str && (!*str || has_whitespace(str)))
			{
				free(str);
				return (ft_substr(input, i, j));
			}
			free(str);
			i = j;
		}
		else
			i++;
	}
	return (NULL);
}
