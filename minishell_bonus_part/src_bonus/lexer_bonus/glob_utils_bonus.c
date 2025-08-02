#include "../../includes_bonus/minishell_bonus.h"

static int	is_dir_pattern(char *pattern)
{
	size_t	len;

	if (!pattern)
		return (0);
	len = ft_strlen(pattern);
	if (len > 0 && pattern[len - 1] == '/')
		return (1);
	return (0);
}

static char	*strip_trailing_slash(char *pattern)
{
	size_t	len;

	if (!pattern)
		return (NULL);
	len = ft_strlen(pattern);
	if (len > 0 && pattern[len - 1] == '/')
	{
		while (pattern[len - 1] == '/')
			len--;
		return (ft_substr(pattern, 0, len));
	}
	return (ft_strdup(pattern));
}

static char	*get_trailing_slash(int must_be_dir, char *word, char *pattern)
{
	char	*t_name;
	size_t	word_len;
	size_t	patt_len;

	if (!must_be_dir || !word || !pattern)
		return (NULL);
	word_len = ft_strlen(word);
	patt_len = ft_strlen(pattern);
	if (word_len < 2 || patt_len < 1)
		return (NULL);
	t_name = ft_substr(word, patt_len, word_len);
	return (t_name);
}

int	init_glob_vars(t_list **words, t_glob *g, char *word)
{
	g->must_be_dir = is_dir_pattern(word);
	g->pattern = strip_trailing_slash(word);
	g->t_name = get_trailing_slash(g->must_be_dir, word, g->pattern);
	g->dir_path = NULL;
	g->ptr = ft_strrchr(g->pattern, '/');
	if (g->ptr)
		g->dir_path = ft_substr(g->pattern, 0, (g->ptr++ - g->pattern) + 1);
	else
		g->ptr = g->pattern;
	if (g->dir_path)
		g->dir = opendir(g->dir_path);
	else
		g->dir = opendir(".");
	if (!g->dir)
	{
		ft_lstadd_back(words, ft_lstnew(ft_strdup(word)));
		free(g->t_name);
		free(g->dir_path);
		free(g->pattern);
		return (0);
	}
	g->f = 0;
	return (1);
}
