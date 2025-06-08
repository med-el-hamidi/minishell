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

static char    *expand_env(t_shell *shell, const char *name)
{
	t_list	*ptr;
	t_env	*env;
    size_t	len;

	ptr = shell->env_list;
	len = ft_strlen(name);
    while (ptr)
    {
        env = ptr->content;
        if (!ft_strncmp(env->key, name, len) && ft_strlen(env->key) == len)
            return (env->value);
		ptr = ptr->next;
    }
    return (NULL);
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
        return (ft_itoa(getpid()));
    }
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

static char *accumulate_quoted(t_shell *shell, char *input, int *i)
{
	char	*result;
	char	*chunk;
	char	temp[2];
	char	*tmp;
	char	quote;

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
        else if (input[*i] == '~' && (!input[*i + 1] || input[*i + 1] == '/' || is_whitespace(input[*i + 1])))
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

        tmp = result;
        result = ft_strjoin(result, chunk);
        free(tmp);
        free(chunk);
    }
    return (result);
}