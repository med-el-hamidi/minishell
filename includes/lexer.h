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

#ifndef LEXER_H
# define LEXER_H

t_token	*create_token(t_token_type type, char *value);
void	add_token(t_list **tokens, t_token *token);
int		is_whitespace(char c);
size_t	skip_whitespace(char *input, size_t *i);
char	*is_ambiguous_redirect(t_shell *shell, char *input, size_t i);

int		check_unclosed_quotes(char *input, size_t i);
char	*ft_strjoin_to_s1(char *s1, char *s2);
char	*ft_strjoin_char_to_s1(char *s1, char c);
char	*gethome(t_list	*vars);

char	*accumulate_dollar(t_shell *shell, char *input, size_t *i);
int		handle_redirection(t_shell *shell, t_list **tokens, char *input, \
																size_t *i);
char	*accumulate_token(t_shell *shell, char *input, size_t *i);

t_list	*lexer(t_shell *shell, char *input);

#endif
