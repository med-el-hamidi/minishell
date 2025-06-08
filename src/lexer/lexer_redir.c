#include "../../includes/minishell.h"

static int  print_syntax_error(const char *token)
{
    ft_putstr_fd("minishell: syntax error near unexpected token '", STDERR_FILENO);
    ft_putstr_fd((char *)token, STDERR_FILENO);
    ft_putstr_fd("'\n", STDERR_FILENO);
    return (2);
}

static int  check_invalid_token(char *input, int pos)
{
	while (is_whitespace(input[pos]))
		pos++;
	if (!ft_isprint(input[pos]))
		return (print_syntax_error("newline"));
	if (input[pos] == '>' || input[pos] == '<')
	{
		if (!ft_strncmp(&input[pos], ">>>", 3))
			return (print_syntax_error(">>>"));
		else if (!ft_strncmp(&input[pos], ">>", 2))
			return (print_syntax_error(">>"));
		else if (!ft_strncmp(&input[pos], "<<<", 3))
			return (print_syntax_error("<<<"));
		else if (!ft_strncmp(&input[pos], "<<", 2))
			return (print_syntax_error("<<"));
		else if (input[pos] == '>')
			return (print_syntax_error(">"));
		else if (input[pos] == '<')
		{
			if (pos - 2 >= 0 && !ft_strncmp(&input[pos - 2], "<<", 2))
				return (0);
			return (print_syntax_error("<"));
		}
	}
	return (0);
}

int handle_redirection(char *input, int *i, t_list **tokens)
{
	int	j;
	int	is_append; 
	int	is_heredoc;

	j = 1;
	is_append = 0;
	is_heredoc = 0;
	if (input[*i] == '>' && input[*i + 1] == '>')
		is_append = 1;
	if ((input[*i] == '<' && input[*i + 1] == '<'))
		is_heredoc = 1;
	if (is_append || is_heredoc)
		j++;
	if (check_invalid_token(input, *i + j))
		return (2);
	else if (is_append)
		add_token(tokens, create_token(TOKEN_REDIR_APPEND, ">>"));
	else if (is_heredoc)
		add_token(tokens, create_token(TOKEN_REDIR_HEREDOC, "<<"));
	else if (input[*i] == '>')
		add_token(tokens, create_token(TOKEN_REDIR_OUT, ">"));
	else
		add_token(tokens, create_token(TOKEN_REDIR_IN, "<"));
	*i += j;
	return (0);
}