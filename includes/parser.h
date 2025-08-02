#ifndef PARSER_H
# define PARSER_H

t_ast	*new_ast_node(int type, char **args);
void	advance_token(t_list **tokens);
int		is_redirection(int type);

t_ast	*parse_redirection(t_list **tokens, t_ast *command);
t_ast	*parse_command(t_list **tokens);

t_ast	*parser(t_list *tokens, t_shell *shell);

#endif
