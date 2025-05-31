# include "../../includes/minishell.h"

t_token *lexer(char *input)
{
	t_token *tokens;
	int i;

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
			handle_redirection(input, &i, &tokens);
		else if (input[i] == '"' || input[i] == '\'')
			handle_quoted_string(input, &i, &tokens);
		else
			handle_word(input, &i, &tokens);
	}
	return (tokens);
}
