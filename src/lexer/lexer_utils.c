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
