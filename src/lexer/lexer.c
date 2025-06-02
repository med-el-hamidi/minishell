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

char	*_getpid()
{
	char	buf[11];
	ssize_t	bytes;
	int		fd;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		return (NULL);
	}
	bytes = read(fd, buf, 10);
	close(fd);
	if (bytes == -1)
	{
		perror("minishell");
		return (NULL);
	}
	else if (!bytes)
		return (NULL);
	return (ft_itoa(ft_atoi(buf)));
}

t_token	get_token_word(t_shell *shell, char *input, int *i, int len)
{
	t_token	token;
	char	*word;

	if (input[*i] == '$')
	{
		if (input[*i + 1] == '$')
		{
			len++;
			token = (t_token){TOKEN_DOLLAR, _getpid()};
		}
		else if (len - 1 == 0)
			token = (t_token){TOKEN_WORD, ft_strdup("$")};
		else
		{
			word = ft_substr(input, *i + 1, len - 1);
			token = (t_token){TOKEN_DOLLAR,
				ft_strdup(expand(shell, word, len - 1))};
			free(word);
		}
	}
	else if (input[*i] == '~' && (!input[*i + 1] || is_whitespace(input[*i + 1])))
		token = (t_token){TOKEN_DOLLAR,
			ft_strdup(expand(shell, "HOME", len - 1))};
	else
		token = (t_token){TOKEN_WORD, ft_substr(input, *i, len)};
	*i += len;
	return (token);
}

void	handle_word(t_shell *shell, char *input, int *i, t_list **tokens)
{
	t_token	token;
	int		len;

	len = 0;
	if (input[*i] == '$')
		len++;
	while (input[*i + len] && !is_special(input[*i + len]))
		len++;
	token = get_token_word(shell, input, i, len);
	add_token(tokens, create_token(token.type, token.value));
	free(token.value);
}


int	handle_redirection(char *input, int *i, t_list **tokens)
{
	int	j;

	j = 0;
	if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			j = 2;
			return (2);
		}
		else
		{
			j = 1;
			while (is_whitespace(input[*i + j]))
				j++;
			if (input[*i + j] == '>')
			{
				if (input[*i + j + 1] == '>')
					ft_putstr_fd("minishell: syntax error near unexpected token '>>'\n", STDERR_FILENO);
				else
					ft_putstr_fd("minishell: syntax error near unexpected token '>'\n", STDERR_FILENO);
				return (2);
			}
			else if (input[*i + j] == '<')
			{
				if (!ft_strncmp(input + *i + j + 1, "<<", 2))
					ft_putstr_fd("minishell: syntax error near unexpected token '<<<'\n", STDERR_FILENO);
				else if (input[*i + j + 1] == '<')
					ft_putstr_fd("minishell: syntax error near unexpected token '<<'\n", STDERR_FILENO);
				else
					ft_putstr_fd("minishell: syntax error near unexpected token '<'\n", STDERR_FILENO);
				return (2);
			}
			else if (!ft_isprint(input[*i + j]))
			{
				ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
				return (2);
			}
			else
			{
				add_token(tokens, create_token(TOKEN_REDIR_OUT, ">"));
			}
		}
	}
	else
	{
		return (2);
	}
	*i += j;
	return (0);
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
			add_token(&tokens, create_token(TOKEN_PIPE, "|"));
			i++;
		}
		else if (input[i] == '>' || input[i] == '<')
		{
			shell->exit_status = handle_redirection(input, &i, &tokens);
			if (shell->exit_status)
			{
				ft_lstclear(&tokens, del_token);
				return (NULL);
			}
		}
		else if (input[i] == '"' || input[i] == '\'')
			;//handle_quoted_string(input, &i, &tokens);
		else
			handle_word(shell, input, &i, &tokens);
	}
	return (tokens);
}
