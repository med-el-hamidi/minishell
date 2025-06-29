/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 18:01:54 by mel-hami          #+#    #+#             */
/*   Updated: 2025/06/29 18:01:55 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*ft_strjoin_in_s1(char *s1, char *s2)
{
	char	*tmp;

	tmp = s1;
	s1 = ft_strjoin(s1, s2);
	free(tmp);
	free(s2);
	return (s1);
}

char	*gethome(t_shell	*shell)
{
	char	*home;
	char	*tmp;

	tmp = _getenv(shell, "HOME");
	if (tmp)
		home = ft_strdup(tmp);
	else
		home = ft_strdup("");
	return (home);
}
