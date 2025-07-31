/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:09:38 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:09:39 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

static int	is_invalid_edge_tokens(t_list *tokens)
{
	t_token	*first;
	t_token	*last;

	first = (t_token *)tokens->content;
	last = (t_token *)ft_lstlast(tokens)->content;
	if (is_control_operator(first->type) || first->type == TOKEN_P_CLOSE)
		return (print_syntax_error(first->value));
	if (is_control_operator(last->type) || last->type == TOKEN_P_OPEN)
		return (print_syntax_error(first->value));
	return (0);
}

static int	check_parenthesis_open(t_list *curr, t_list *prev)
{
	t_token	*token;
	t_token	*next;

	token = (t_token *)curr->content;
	if (!curr->next)
		return (print_syntax_error(token->value));
	else if (prev && !is_control_operator(((t_token *)prev->content)->type)
		&& ((t_token *)prev->content)->type != TOKEN_P_OPEN)
		return (print_syntax_error(token->value));
	next = (t_token *)curr->next->content;
	if (next->type == TOKEN_P_CLOSE || is_control_operator(next->type))
		return (print_syntax_error(next->value));
	return (0);
}

static int	check_parenthesis_close(t_list *curr, t_list *prev)
{
	t_token	*token;
	t_token	*next;

	token = (t_token *)curr->content;
	if (prev && is_control_operator(((t_token *)prev->content)->type))
		return (print_syntax_error(token->value));
	else if (!curr->next)
		return (0);
	next = (t_token *)curr->next->content;
	if (next->type == TOKEN_P_OPEN || next->type == TOKEN_WORD)
		return (print_syntax_error(next->value));
	return (0);
}

static int	check_error(t_list *curr, t_list *prev, int *level)
{
	t_token	*token;

	token = (t_token *)curr->content;
	if (token->type == TOKEN_P_OPEN)
	{
		(*level)++;
		return (check_parenthesis_open(curr, prev));
	}
	else if (token->type == TOKEN_P_CLOSE)
	{
		(*level)--;
		if (*level < 0)
			return (print_syntax_error(token->value));
		return (check_parenthesis_close(curr, prev));
	}
	else if (is_control_operator(token->type) && prev
		&& is_control_operator(((t_token *)prev->content)->type))
		return (print_syntax_error(token->value));
	else if (is_redirection(token->type) && !token->value)
		return (SNTX_EXIT_STATUS);
	return (0);
}

int	syntax_error(t_list *tokens)
{
	t_list	*curr;
	t_list	*prev;
	int		paren_level;

	if (!tokens)
		return (1);
	if (is_invalid_edge_tokens(tokens))
		return (SNTX_EXIT_STATUS);
	curr = tokens;
	prev = NULL;
	paren_level = 0;
	while (curr)
	{
		if (check_error(curr, prev, &paren_level))
			return (SNTX_EXIT_STATUS);
		prev = curr;
		curr = curr->next;
	}
	if (paren_level != 0)
		return (
			ft_putstr_fd("minishell: syntax error: unmatched parenthesis!\n",
				STDERR_FILENO), SNTX_EXIT_STATUS);
	return (0);
}
