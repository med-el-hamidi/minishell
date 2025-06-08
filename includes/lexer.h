#ifndef LEXER_H
# define LEXER_H

t_token	*create_token(t_token_type type, char *value);
void	add_token(t_list **tokens, t_token *token);
int		is_whitespace(char c);
int		is_special(char c);

int		handle_redirection(char *input, int *i, t_list **tokens);
char	*accumulate_token(t_shell *shell, char *input, int *i);

t_list	*lexer(t_shell *shell, char *input);

#endif
