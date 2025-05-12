/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hami <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 19:33:34 by mel-hami          #+#    #+#             */
/*   Updated: 2024/12/09 19:33:36 by mel-hami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	found_new_line(t_list *list)
{
	int	i;

	while (list)
	{
		i = 0;
		while (list->buffer[i])
		{
			if ('\n' == list->buffer[i])
				return (1);
			i++;
		}
		list = list->next;
	}
	return (0);
}

int	get_length_new_line(t_list *list)
{
	int	i;
	int	len;

	len = 0;
	while (list)
	{
		i = 0;
		while (list->buffer[i])
		{
			if ('\n' == list->buffer[i])
			{
				len++;
				return (len);
			}
			len++;
			i++;
		}
		list = list->next;
	}
	return (len);
}

void	safe_free(t_list **list, t_list *node)
{
	ft_lstclear(list);
	if (node->buffer[0])
		*list = node;
	else
	{
		free(node->buffer);
		free(node);
	}
}
