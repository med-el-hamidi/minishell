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
	token->value = ft_strdup(value);
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

int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>'
		|| c == '"' || c == '\'' || c == '$');
}

char	*expand_env(t_shell *shell, const char *name)
{
	t_list	*ptr;
	t_env	*env;
	size_t	len;

	len = ft_strlen(name);
	ptr = shell->env_list;
	while (ptr)
	{
		env = ptr->content;
		if (!ft_strncmp(env->key, name, len) && ft_strlen(env->key) == len)
			return (env->value);
		ptr = ptr->next;
	}
	ptr = shell->vars;
	while (ptr)
	{
		env = ptr->content;
		if (!ft_strncmp(env->key, name, len) && ft_strlen(env->key) == len)
			return (env->value);
		ptr = ptr->next;
	}
	return (NULL);
}
