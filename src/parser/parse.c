/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 19:09:09 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 19:09:11 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	_count_args(t_list **tokens)
{
	int		count;
	t_list	*tmp;

	count = 0;
	tmp = *tokens;
	while (tmp && ((t_token *)tmp->content)->type != TOKEN_PIPE)
	{
		if (((t_token *)tmp->content)->type == TOKEN_WORD)
			count++;
		tmp = tmp->next;
	}
	return (count);
}

static char	**gather_args(t_list **tokens, t_ast **redir_chain, t_shell *shell)
{
	char	**args;
	size_t	i;

	args = malloc(sizeof(char *) * (_count_args(tokens) + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (*tokens && ((t_token *)(*tokens)->content)->type != TOKEN_PIPE)
	{
		if (((t_token *)(*tokens)->content)->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(((t_token *)(*tokens)->content)->value);
			if (!args[i])
				return (free_2d_array(args), NULL);
			advance_token(tokens);
		}
		else if (is_redirection(((t_token *)(*tokens)->content)->type))
		{
			*redir_chain = parse_redirection(tokens, *redir_chain, shell);
			if (!*redir_chain)
				return (free_2d_array(args), NULL);
		}
		args[++i] = NULL;
	}
	return (args);
}

static t_ast	*_link_leading_redir_to_cmd(t_ast *redir_chain, t_ast *command)
{
	t_ast	*last;

	if (redir_chain)
	{
		last = redir_chain;
		while (last->left)
			last = last->left;
		last->left = command;
		return (redir_chain);
	}
	return (command);
}

t_ast	*parse_command(t_list **tokens, t_shell *shell)
{
	t_ast	*command;
	t_ast	*redir_chain;
	char	**args;

	command = NULL;
	redir_chain = NULL;
	while (*tokens && is_redirection(((t_token *)(*tokens)->content)->type))
	{
		redir_chain = parse_redirection(tokens, redir_chain, shell);
		if (!redir_chain)
			return (NULL);
	}
	if (!*tokens || ((t_token *)(*tokens)->content)->type != TOKEN_WORD)
	{
		if (redir_chain)
			return (redir_chain);
		return (NULL);
	}
	args = gather_args(tokens, &redir_chain, shell);
	if (!args)
		return (NULL);
	command = new_ast_node(AST_CMD, args);
	if (!command)
		return (free_2d_array(args), NULL);
	return (_link_leading_redir_to_cmd(redir_chain, command));
}
