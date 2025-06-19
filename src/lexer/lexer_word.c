#include "../../includes/minishell.h"

static char    *accumulate_word(char *input, int *i)
{
	char	*result;
	char	temp[2];
	char	*tmp;

	temp[1] = 0;
	result = ft_strdup("");
	while (input[*i] && !is_whitespace(input[*i]) && !is_special(input[*i]))
	{
		temp[0] = input[*i];
		tmp = result;
		result = ft_strjoin(result, temp);
		free(tmp);
		(*i)++;
	}
	return (result);
}

char	*_getpid()
{
	char	buf[11];
	ssize_t	bytes;
	int		fd;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: getpid");
		return (NULL);
	}
	bytes = read(fd, buf, 10);
	close(fd);
	if (bytes == -1)
	{
		perror("minishell: getpid");
		return (NULL);
	}
	else if (!bytes)
		return (NULL);
	return (ft_itoa(ft_atoi(buf)));
}

static char *accumulate_dollar(t_shell *shell, char *input, int *i)
{
	int		start;
	char	*key;
	char	*val;

	(*i)++; // skip $
	if (input[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(shell->exit_status));
	}
	else if (input[*i] == '$')
	{
		(*i)++;
		return (_getpid());
	}
	else if (!input[*i] || is_whitespace(input[*i]))
		return (ft_strdup("$"));
	start = *i;
	while (ft_isalnum(input[*i]) || input[*i] == '_')
		(*i)++;
	key = ft_substr(input, start, *i - start);
	val = expand_env(shell, key);
	free(key);
	if (val)
		return ft_strdup(val);
	return (ft_strdup(""));
}

static int	check_unclosed_quotes(char *input, int i)
{
	if (input[i++] == '\'')
	{
		while (input[i] && input[i] != '\'')
			i++;
		if (input[i] == '\'')
			return (0);
	}
	else
	{
		while (input[i] && input[i] != '"')
			i++;
		if (input[i] == '"')
			return (0);
	}
	ft_putstr_fd("minishell: syntax error: unclosed quotes!\n", STDERR_FILENO);
	return (2);
}

static char *accumulate_quoted(t_shell *shell, char *input, int *i)
{
	char	*result;
	char	*chunk;
	char	temp[2];
	char	*tmp;
	char	quote;

	if (check_unclosed_quotes(input, *i))
		return (NULL);
	temp[1] = 0;
	result = ft_strdup("");
	quote = input[(*i)++];
	while (input[*i] && input[*i] != quote)
	{
		chunk = NULL;
		if (quote == '"' && input[*i] == '$')
			chunk = accumulate_dollar(shell, input, i);
		else
		{
			temp[0] = input[*i];
			chunk = ft_strdup(temp);
			(*i)++;
		}
		tmp = result;
		result = ft_strjoin(result, chunk);
		free(tmp);
		free(chunk);
	}
	if (input[*i] == quote)
		(*i)++;
	return (result);
}

char	*accumulate_token(t_shell *shell, char *input, int *i)
{
	char	*result;
	char	*chunk;
	char	*tmp;

	result = ft_strdup("");
	while (input[*i] && !is_whitespace(input[*i]) && !ft_strchr("|<>", input[*i]))
	{
		chunk = NULL;
		if (input[*i] == '"' || input[*i] == '\'')
			chunk = accumulate_quoted(shell, input, i);
		else if (input[*i] == '$')
			chunk = accumulate_dollar(shell, input, i);
		else if (input[*i] == '~' && (!input[*i + 1] || input[*i + 1] == '/'
				|| is_whitespace(input[*i + 1])))
		{
			(*i)++;
			tmp = expand_env(shell, "HOME");
			if (tmp)
				chunk = ft_strdup(tmp);
			else
				chunk = ft_strdup("");
		}
		else
			chunk = accumulate_word(input, i);
		if (!chunk)
		{
			free(result);
			return (NULL);
		}
		tmp = result;
		result = ft_strjoin(result, chunk);
		free(tmp);
		free(chunk);
	}
	return (result);
}
