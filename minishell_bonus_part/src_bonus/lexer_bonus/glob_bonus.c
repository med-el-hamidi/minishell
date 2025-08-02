#include "../../includes_bonus/minishell_bonus.h"

static int	match(char *pattern, char *name)
{
	if (*pattern == '\0' && *name == '\0')
		return (1);
	if (*pattern == '*')
	{
		return (match(pattern + 1, name) || \
			(*name && match(pattern, name + 1)));
	}
	if (*pattern == *name)
		return (match(pattern + 1, name + 1));
	return (0);
}

static void	process_entry(t_list **words, t_glob *g)
{
	char	*tmp;

	if (*g->ptr != '.' && g->entry->d_name[0] == '.')
		return ;
	else if (!match(g->ptr, g->entry->d_name))
		return ;
	else if (g->must_be_dir && !is_directory(g->entry->d_name))
		return ;
	g->f = 1;
	if (g->t_name)
		g->h_name = ft_strjoin(g->entry->d_name, g->t_name);
	else
		g->h_name = g->entry->d_name;
	if (g->dir_path)
	{
		tmp = g->h_name;
		g->h_name = ft_strjoin(g->dir_path, g->h_name);
		ft_lstadd_back(words, ft_lstnew(ft_strdup(g->h_name)));
		if (g->t_name)
			free(tmp);
	}
	else
		ft_lstadd_back(words, ft_lstnew(ft_strdup(g->h_name)));
	if (g->t_name || g->dir_path)
		free(g->h_name);
}

static void	free_glob_vars(t_glob *g)
{
	free(g->t_name);
	free(g->dir_path);
	free(g->pattern);
	closedir(g->dir);
}

t_list	*handle_glob(char *word)
{
	t_list	*words;
	t_glob	g;

	if (!word)
		return (NULL);
	words = NULL;
	if (!init_glob_vars(&words, &g, word))
		return (words);
	while (1)
	{
		g.entry = readdir(g.dir);
		if (!g.entry)
			break ;
		process_entry(&words, &g);
	}
	if (!g.f)
		ft_lstadd_back(&words, ft_lstnew(ft_strdup(word)));
	free_glob_vars(&g);
	return (words);
}
