/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_set.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 15:36:42 by mel-hami          #+#    #+#             */
/*   Updated: 2025/07/13 15:36:43 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	ft_issep(char ch, char *seps)
{
	while (*seps)
	{
		if (ch == *seps)
			return (1);
		seps++;
	}
	return (0);
}

static size_t	ft_cwords(char const *s, char *seps)
{
	size_t	words;
	int		flag;

	words = 0;
	flag = 0;
	while (*s)
	{
		if (!ft_issep(*s, seps) && !flag)
		{
			flag = 1;
			words++;
		}
		else if (ft_issep(*s, seps))
			flag = 0;
		s++;
	}
	return (words);
}

static size_t	ft_sublen(char const *s, char *seps)
{
	size_t	i;

	i = 0;
	while (s[i] && !ft_issep(s[i], seps))
		i++;
	return (i);
}

static void	ft_free(char **arr, size_t index)
{
	size_t	i;

	i = 0;
	while (i < index)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

char	**ft_split_set(char const *s, char *seps)
{
	char	**res;
	size_t	i;
	size_t	len;

	if (!s || !seps)
		return (NULL);
	res = (char **)malloc(sizeof(char *) * (ft_cwords(s, seps) + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (!ft_issep(*s, seps))
		{
			len = ft_sublen(s, seps);
			res[i] = (char *)malloc(sizeof(char) * (len + 1));
			if (!res[i])
				return (ft_free(res, i), NULL);
			ft_strlcpy(res[i++], s, len + 1);
			s += len;
		}
		else
			s++;
	}
	return (res[i] = NULL, res);
}
