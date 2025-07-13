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

static int	lexer_word(t_shell	*shell, t_list **tokens, char *input, size_t *i)
{
	char	**content;
	char	*word;
	char	*tmp;
	size_t	j;
	size_t	len;
	int		f;

	f = 1;
	word = NULL;
	while (input[*i] && !is_whitespace(input[*i])
		&& !ft_strchr("|<>", input[*i]))
	{
		if (input[*i] == '$')
		{
			f &= 1;
			tmp = accu_dollar(shell, input, i, _getenv_al);
			if (!has_whitespace(tmp))
			{
				word = ft_strjoin_to_s1(word, tmp);
				continue ;
			}
			content = ft_split(tmp, ' ');
			if (!content)
				return (free(tmp), free (word), ft_lstclear(tokens, del_token), 0);
			if (word && tmp && tmp[0] == ' ')
			{
				add_token(tokens, create_token(TOKEN_WORD, word));
				free(word);
				word = NULL;
			}
			j = 0;
			if (word && content[j])
			{
				word = ft_strjoin_to_s1(word, ft_strdup(content[j++]));
				add_token(tokens, create_token(TOKEN_WORD, word));
				free(word);
				word = NULL;
			}
			while (content[j] && content[j + 1])
				add_token(tokens, create_token(TOKEN_WORD, content[j++]));
			if (content[j])
			{
				len = ft_strlen(tmp);
				if (len && tmp[len - 1] == ' ')
					add_token(tokens, create_token(TOKEN_WORD, content[j++]));
				if (content[j])
					word = ft_strdup(content[j]);
				else
					word = ft_strdup("");
			}
			(free(tmp), free_2d_array(content));
		}
		else if (input[*i] == '~' && (!input[*i + 1] || input[*i + 1] == '/'
				|| is_whitespace(input[*i + 1])))
		{
			f &= 1;
			(*i)++;
			word = ft_strjoin_to_s1(word, gethome(shell->vars));
		}
		else
		{
			f &= 0;
			tmp = accumulate_token(shell, input, i);
			if (!tmp && word)
				free(word);
			word = ft_strjoin_to_s1(word, tmp);
		}
		if (!word)
			break ;
	}
	if (word && !*word && f)
		return (free(word), 1);
	else if (word)
		add_token(tokens, create_token(TOKEN_WORD, word));
	else
		return (ft_lstclear(tokens, del_token), 0);
	return (free(word), 1);
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
