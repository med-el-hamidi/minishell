/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:20:21 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:20:22 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_BONUS_H
# define LEXER_BONUS_H

t_token	*create_token(t_token_type type, char *value);
void	add_token(t_list **tokens, t_token *token);

char	**ft_split_set(char const *s, char *seps);

int		check_invalid_token(char *input, size_t pos);
char	*get_redir_filename(t_shell *shell, char *input, size_t *i);
int		is_whitespace(char c);
size_t	skip_whitespace(char *input, size_t *i);
int		has_whitespace(char *str);
char	*is_ambiguous_redirect(t_shell *shell, char *input, size_t i);

int		should_parse_dollar(char *input, size_t i);
char	*get_delimiter(char *input, size_t	*i, int f);
char	*get_redir_filename(t_shell *shell, char *input, size_t *i);

int		check_unclosed_quotes(char *input, size_t i);
char	*ft_strjoin_to_s1(char *s1, char *s2);
char	*ft_strjoin_char_to_s1(char *s1, char c);
char	*_getenv_al(t_list *vars, char *name);
char	*gethome(t_list	*vars);

int		handle_redirection(t_shell *shell, t_list **tokens, char *input, \
																size_t *i);
char	*accumulate_word(char *input, size_t *i);
char	*accumulate_quoted(t_shell *shell, char *input, size_t *i);
char	*accumulate_dollar(t_shell *shell, char *input, size_t *i, \
												char *f(t_list *, char *));
char	*accumulate_other(t_shell *shell, char *input, size_t *i, int *f);

int		handle_lexer_word(t_lexerctx *ctx, char **word);

int		init_glob_vars(t_list **words, t_glob *g, char *word);
t_list	*handle_glob(char *word);
void	add_token_word(t_lexerctx *ctx, char *word);

t_list	*lexer(t_shell *shell, char *input);

#endif
