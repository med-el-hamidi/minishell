/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:20:28 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/28 21:20:30 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

t_ast	*new_ast_node(int type, char **args);
void	advance_token(t_list **tokens);
int		is_redirection(int type);
char	*expand_env(t_shell *shell, const char *name);

t_ast	*parse_command(t_list **tokens);

t_ast	*parser(t_list *tokens);

#endif
