# include "../../includes/minishell.h"

int	is_whitespace(char c)
{
	return ((c >= 9 && c <= 13) || c == ' ');
}

int	is_special(char c)
{
	return (is_whitespace(c) || c == '$' || c == '|' ||
		c == '>' || c == '<' ||
		c == '"' || c == '\'');
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	if (!value)
		return (NULL);
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

char	*expand(t_shell *shell, char *word, int len)
{
	t_list	*ptr;

	ptr = shell->env_list;
	while (ptr)
	{
		if (!ft_strncmp(((t_env *)ptr->content)->key, word, len))
			return (((t_env *)ptr->content)->value);
		ptr = ptr->next;
	}
	return (NULL);
}

void	handle_word(t_shell *shell, char *input, int *i, t_list **tokens)
{
	char	*word;
	int		len;

	word = NULL;
	len = 0;
	if (input[*i + len] == '$')
		len++;
	while (input[*i + len] && !is_special(input[*i + len]))
		len++;
	if (input[*i] == '$')
	{
		if (input[*i + 1] == '$')
		{
			word = "123456";
			add_token(tokens, create_token(TOKEN_WORD, word));
		}
		else if (len - 1 == 0)
			add_token(tokens, create_token(TOKEN_WORD, "$"));
		else
		{
			word = ft_substr(input, *i + 1, len - 1);
			add_token(tokens, create_token(TOKEN_DOLLAR, expand(shell, word, len - 1)));
		}
	}
	else
	{
		word = ft_substr(input, *i, len);
		add_token(tokens, create_token(TOKEN_WORD, word));
	}
	*i += len;
	free(word);
}

t_list	*lexer(t_shell *shell, char *input)
{
	t_list	*tokens;
	int		i;

	if (!shell || !input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (is_whitespace(input[i]))
			i++;
		else if (input[i] == '|')
		{
			//add_token(&tokens, create_token(TOKEN_PIPE, "|"));
			i++;
		}
		else if (input[i] == '>' || input[i] == '<')
			;//handle_redirection(input, &i, &tokens);
		else if (input[i] == '"' || input[i] == '\'')
			;//handle_quoted_string(input, &i, &tokens);
		else
			handle_word(shell, input, &i, &tokens);
	}
	return (tokens);
}
