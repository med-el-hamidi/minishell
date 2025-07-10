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
	char	*word;

	word = accumulate_token(shell, input, i);
	if (word)
		add_token(tokens, create_token(TOKEN_WORD, word));
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
	if (shell->exit_status == 130)
		g_exit_status = 130;
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
