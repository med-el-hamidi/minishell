#include "../../includes/minishell.h"

int	should_parse_dollar(char *input, size_t i)
{
	while (input[i] && !ft_strchr("|<>", input[i]))
	{
		if ((input[i] == '"' || input[i] == '\''))
			return (0);
		i++;
	}
	return (1);
}

char	*get_delimiter(char *input, size_t	*i, int f)
{
	char	*result;

	result = NULL;
	while (input[*i])
	{
		if (!f && (is_whitespace(input[*i]) || ft_strchr("|<>", input[*i])))
			break ;
		else if (input[*i] == '"' || input[*i] == '\'')
		{
			if (!f && check_unclosed_quotes(input, *i))
				return (free(result), NULL);
			f = !f;
			(*i)++;
			result = ft_strjoin_to_s1(result, ft_strdup(""));
		}
		else if (!f && input[*i] == '$'
			&& (input[*i + 1] == '"' || input[*i + 1] == '\''))
			(*i)++;
		else
			result = ft_strjoin_char_to_s1(result, input[(*i)++]);
	}
	return (result);
}
