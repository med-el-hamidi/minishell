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
int		is_special(char c);

char	*ft_strjoin_in_s1(char *s1, char *s2);
char	*gethome(t_shell	*shell);

int		handle_redirection(char *input, int *i, t_list **tokens);
char	*accumulate_token(t_shell *shell, char *input, int *i);

t_list	*lexer(t_shell *shell, char *input);

#endif
