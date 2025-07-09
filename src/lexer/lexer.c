/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 16:36:51 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 16:36:52 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	match(char *pattern, char *name)
{
	// Base cases
	if (*pattern == '\0' && *name == '\0')
		return (1);
	if (*pattern == '*')
	{
		// Try to match zero or more chars
		return (match(pattern + 1, name) ||
			(*name && match(pattern, name + 1)));
	}
	if (*pattern == *name)
		return match(pattern + 1, name + 1);
    return (0);
}

static void	handle_glob(t_list **tokens, char *word)
{
	DIR				*dir;
	struct dirent	*entry;
	//struct stat		*state;
	char			*d_name;
	char			*dir_path;
	char			*ptr;
	int				f;

	if (!tokens || !word)
		return ;
	dir_path = NULL;
	ptr = ft_strrchr(word, '/');
	if (ptr)
	{
		dir_path = ft_substr(word, 0, (ptr - word) + 1);
		ptr++;
	}
	else
		ptr = word;
	if (dir_path)
	{
		dir = opendir(dir_path);
		if (!dir)
		{
			add_token(tokens, create_token(TOKEN_WORD, word));
			free(dir_path);
			return ;
		}
	}
	else
	{
		dir = opendir(".");
		if (!dir)
			return ;
	}
	f = 0;
	while (1)
	{
		entry = readdir(dir);

		if (!entry)
			break ;
		if (*ptr != '.' && !ft_strncmp(entry->d_name, ".", 1))
			continue ;
		if (match(ptr, entry->d_name))
		{
			f = 1;
			if (dir_path)
			{
				d_name = ft_strjoin(dir_path, entry->d_name);
				add_token(tokens, create_token(TOKEN_WORD, d_name));
				free(d_name);
			}
			else
				add_token(tokens, create_token(TOKEN_WORD, entry->d_name));
		}
	}
	if (!f)
		add_token(tokens, create_token(TOKEN_WORD, word));
	if (dir_path)
		free(dir_path);
	closedir(dir);
}

static int	lexer_word(t_shell	*shell, t_list **tokens, char *input, size_t *i)
{
	char	*word;

	word = accumulate_token(shell, input, i);
	if (word)
	{
		if (ft_strchr(word, '*'))
			handle_glob(tokens, word);
		else
			add_token(tokens, create_token(TOKEN_WORD, word));
	}
	else
	{
		free(word);
		ft_lstclear(tokens, del_token);
		return (0);
	}
	free(word);
	return (1);
}

static int	lexer_redir(t_shell	*shell, t_list **tokens, char *input, size_t *i)
{
	shell->exit_status = handle_redirection(shell, tokens, input, i);
	if (shell->exit_status)
	{
		ft_lstclear(tokens, del_token);
		return (0);
	}
	return (1);
}

t_list	*lexer(t_shell *shell, char *input)
{
	t_list	*tokens;
	size_t	i;

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
			if (!lexer_redir(shell, &tokens, input, &i))
				return (NULL);
		}
		else if (!lexer_word(shell, &tokens, input, &i))
			return (NULL);
	}
	return (tokens);
}
