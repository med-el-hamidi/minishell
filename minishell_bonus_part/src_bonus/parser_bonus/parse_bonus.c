/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 19:09:09 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 19:09:11 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes_bonus/minishell_bonus.h"

static t_ast	*parse_subshell(t_list	**tokens)
{
	t_ast	*left;
	t_ast	*subshell;
	t_ast	*redir_chain;

	if (!tokens || !*tokens)
		return (NULL);
	advance_token(tokens);
	left = build_ast_wraper(tokens);
	if (!left || !*tokens
		|| ((t_token *)(*tokens)->content)->type != TOKEN_P_CLOSE)
		return (free_ast(left), NULL);
	advance_token(tokens);
	subshell = new_ast_node(AST_SUBSHELL, NULL);
	if (!subshell)
		return (free_ast(left), NULL);
	subshell->left = left;
	redir_chain = NULL;
	while (*tokens && is_redirection(((t_token *)(*tokens)->content)->type))
	{
		redir_chain = parse_redirection(tokens, redir_chain);
		if (!redir_chain)
			return (free_ast(subshell), NULL);
	}
	return (_link_leading_redir_to_cmd(redir_chain, subshell));
}

static int	_count_args(t_list **tokens)
{
	int		count;
	t_list	*tmp;

	if (!tokens)
		return (0);
	count = 0;
	tmp = *tokens;
	while (tmp && !is_special(((t_token *)(*tokens)->content)->type))
	{
		if (((t_token *)tmp->content)->type == TOKEN_WORD)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

static int	add_new_arg(char **args, size_t *i, char *value)
{
	if (value)
	{
		args[*i] = ft_strdup(value);
		if (!args[*i])
			return (0);
		args[++(*i)] = NULL;
	}
	return (1);
}

static char	**gather_args(t_list **tokens, t_ast **redir_chain)
{
	char	**args;
	size_t	i;

	args = malloc(sizeof(char *) * (_count_args(tokens) + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (*tokens && !is_special(((t_token *)(*tokens)->content)->type))
	{
		if (((t_token *)(*tokens)->content)->type == TOKEN_WORD)
		{
			if (!add_new_arg(args, &i, ((t_token *)(*tokens)->content)->value))
				return (free_2d_array(args), NULL);
			advance_token(tokens);
		}
		else if (is_redirection(((t_token *)(*tokens)->content)->type))
		{
			*redir_chain = parse_redirection(tokens, *redir_chain);
			if (!*redir_chain)
				return (free_2d_array(args), NULL);
		}
	}
	return (args[i] = NULL, args);
}

t_ast	*parse_command(t_list **tokens)
{
	t_ast	*command;
	t_ast	*redir_chain;
	char	**args;

	if (((t_token *)(*tokens)->content)->type == TOKEN_P_OPEN)
		return (parse_subshell(tokens));
	redir_chain = parse_leading_redirection(tokens);
	if (!*tokens || ((t_token *)(*tokens)->content)->type != TOKEN_WORD)
	{
		if (redir_chain)
			return (redir_chain);
		return (NULL);
	}
	args = gather_args(tokens, &redir_chain);
	if (!args)
		return (NULL);
	command = new_ast_node(AST_CMD, args);
	if (!command)
		return (free_2d_array(args), NULL);
	return (_link_leading_redir_to_cmd(redir_chain, command));
}
