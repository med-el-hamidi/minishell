#ifndef PARSER_BONUS_H
# define PARSER_BONUS_H

t_ast	*new_ast_node(int type, char **args);
void	advance_token(t_list **tokens);
int		is_redirection(int type);
int		is_control_operator(t_token_type type);
int		is_special(int type);

int		syntax_error(t_list *tokens);
t_ast	*parse_redirection(t_list **tokens, t_ast *command);
t_ast	*parse_leading_redirection(t_list **tokens);
t_ast	*_link_leading_redir_to_cmd(t_ast *redir_chain, t_ast *command);
t_ast	*parse_command(t_list **tokens);
t_ast	*build_ast_wraper(t_list **tokens);
t_ast	*parser(t_list *tokens, t_shell *shell);

#endif
