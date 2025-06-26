#include "../../includes/minishell.h"

static t_ast	*parse_redirection(t_list **tokens, t_ast *command)
{
	t_token	*redir_tok;
	t_token	*file_tok;
	t_ast	*redir_node;

	while (*tokens && is_redirection(((t_token *)(*tokens)->content)->type))
	{
		redir_tok = (t_token *)(*tokens)->content;
		advance_token(tokens);
		if (!*tokens || ((t_token *)(*tokens)->content)->type != TOKEN_WORD)
			return (NULL);
		file_tok = (t_token *)(*tokens)->content;
		redir_node = new_ast_node(AST_REDIR, NULL);
		if (!redir_node)
			return (NULL);
		redir_node->left = command;
		redir_node->redir_file = ft_strdup(file_tok->value);
		if (!redir_node->redir_file)
		{
			free(redir_node);
			return (NULL);
		}
		redir_node->redir_fd = -1;
		if (redir_tok->type == TOKEN_REDIR_IN)
			redir_node->redir_type = REDIR_INPUT;
		else if (redir_tok->type == TOKEN_REDIR_OUT)
			redir_node->redir_type = REDIR_OUTPUT;
		else if (redir_tok->type == TOKEN_REDIR_APPEND)
			redir_node->redir_type = REDIR_APPEND;
		else if (redir_tok->type == TOKEN_REDIR_HEREDOC)
			redir_node->redir_type = REDIR_HEREDOC;
		command = redir_node;
		set_herdoc_tmp_file(command);
		advance_token(tokens);
	}
	return (command);
}

static char	**gather_arguments(t_list **tokens, t_ast	**redir_chain)
{
	char	**args;
	size_t	count;
	t_list	*tmp;
	size_t	i;

	count = 0;
	tmp = *tokens;
	while (tmp && ((t_token *)tmp->content)->type != TOKEN_PIPE)
	{
		if (((t_token *)tmp->content)->type == TOKEN_WORD)
			count++;
		else if (is_redirection(((t_token *)tmp->content)->type))
			tmp = tmp->next;
		tmp = tmp->next;
	}
	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (*tokens &&  ((t_token *)(*tokens)->content)->type != TOKEN_PIPE)
	{
		if (((t_token *)(*tokens)->content)->type == TOKEN_WORD)
		{
			args[i] = ft_strdup(((t_token *)(*tokens)->content)->value);
			if (!args[i])
			{
				free_2d_array(args);
				return (NULL);
			}
			i++;
			advance_token(tokens);
		}
		else if (is_redirection(((t_token *)(*tokens)->content)->type))
			*redir_chain = parse_redirection(tokens, *redir_chain);
	}
	args[i] = NULL;
	return (args);
}

t_ast	*parse_command(t_list **tokens)
{
	t_ast	*command;
	t_ast	*redir_chain;
	t_ast	*last;
	char	**args;

	command = NULL;
	redir_chain = NULL;
	// Handle leading redirections
	while (*tokens && is_redirection(((t_token *)(*tokens)->content)->type))
	{
		redir_chain = parse_redirection(tokens, redir_chain);
		if (!redir_chain)
			return (NULL);
	}
	// Check for actual command (WORD)
	if (!*tokens || ((t_token *)(*tokens)->content)->type != TOKEN_WORD)
	{
		if (redir_chain)
			return (redir_chain);
		return (NULL);
	}
	// Gather command and arguments
	args = gather_arguments(tokens, &redir_chain);
	if (!args)
		return (NULL);
	command = new_ast_node(AST_CMD, args);
	if (!command)
	{
		free_2d_array(args);
		return (NULL);
	}
	// Apply trailing redirections after command
	command = parse_redirection(tokens, command);
	// Link leading redirection chain (if any) to command
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
