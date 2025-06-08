#ifndef PARSER_H
# define PARSER_H

t_ast	*new_ast_node(int type, char **args);
void	advance_token(t_list **tokens);
int		is_redirection(int type);
char	*expand_env(t_shell *shell, const char *name);

t_ast	*parse_command(t_list **tokens);

t_ast	*parser(t_list *tokens);

#endif
