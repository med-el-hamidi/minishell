#include "../../includes/minishell.h"

t_list	*lexer(t_shell *shell, char *input)
{
	t_list	*tokens;
	char	*word;
	int		i;

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
		else if (input[i] == '<' || input[i] == '>')
		{
			shell->exit_status = handle_redirection(input, &i, &tokens);
			if (shell->exit_status)
			{
				ft_lstclear(&tokens, del_token);
				return (NULL);
			}
		}
		else
		{
			word = accumulate_token(shell, input, &i);
			if (word)
				add_token(&tokens, create_token(TOKEN_WORD, word));
			else
			{
				free(word);
				ft_lstclear(&tokens, del_token);
				return (NULL);
			}
			free(word);
		}
	}
	return (tokens);
}
