#include "../../includes/minishell.h"

t_token *create_token(t_token_type type, char *value)
{
    t_token *token;
	
	token = malloc(sizeof(t_token));
    if (!token)
		return NULL;
    token->type = type;
    token->value = ft_strdup(value);
    return token;
}

void add_token(t_list **tokens, t_token *token)
{
	if (!tokens || !token)
		return;
	ft_lstadd_back(tokens, ft_lstnew(token));
}

int is_whitespace(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

int is_special(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '"' || c == '\'' || c == '$');
}