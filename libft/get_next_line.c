/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: obensarj <obensarj@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 19:33:17 by mel-hami          #+#    #+#             */
/*   Updated: 2025/07/18 08:11:26 by obensarj         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	_create_list(t_list **list, int fd);
static void	_listadd_back(t_list **list, char *buffer);
static char	*_gnl(t_list *list);
static void	_getready_next_gnl(t_list **list);

char	*get_next_line(int fd)
{
	static t_list	*list;
	char			*next_line;

	if (fd < 0 || fd >= OPEN_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	_create_list(&list, fd);
	if (!list)
		return (NULL);
	next_line = _gnl(list);
	_getready_next_gnl(&list);
	return (next_line);
}

static void	_create_list(t_list **list, int fd)
{
	char	*buffer;
	ssize_t	bytes_readed;

	while (!found_new_line(*list))
	{
		buffer = malloc((size_t)BUFFER_SIZE + 1);
		if (!buffer)
			return ;
		bytes_readed = read(fd, buffer, BUFFER_SIZE);
		if (bytes_readed <= 0)
		{
			if (bytes_readed == -1)
				ft_lstclear(list, free);
			free(buffer);
			return ;
		}
		buffer[bytes_readed] = '\0';
		_listadd_back(list, buffer);
	}
}

static void	_listadd_back(t_list **list, char *buffer)
{
	t_list	*node;
	t_list	*last_node;

	node = malloc(sizeof(t_list));
	if (!node)
	{
		free(buffer);
		return ;
	}
	node->content = buffer;
	node->next = NULL;
	last_node = ft_lstlast(*list);
	if (last_node)
		last_node->next = node;
	else
		*list = node;
}

static char	*_gnl(t_list *list)
{
	char	*next_line;
	int		i;
	int		j;

	next_line = malloc(get_length_new_line(list) + 1);
	if (!next_line)
		return (NULL);
	j = 0;
	while (list)
	{
		i = 0;
		while (((char *)list->content)[i])
		{
			next_line[j++] = ((char *)list->content)[i];
			if ('\n' == ((char *)list->content)[i++])
			{
				next_line[j] = '\0';
				return (next_line);
			}
		}
		list = list->next;
	}
	next_line[j] = '\0';
	return (next_line);
}

static void	_getready_next_gnl(t_list **list)
{
	t_list	*lst;
	t_list	*node;
	char	*buf;
	int		i;
	int		j;

	if (!(*list))
		return ;
	buf = malloc((size_t)BUFFER_SIZE + 1);
	if (!buf)
		return ;
	node = malloc(sizeof(t_list));
	if (!node)
		return ;
	lst = ft_lstlast(*list);
	i = 0;
	j = 0;
	while (((char *)lst->content)[i] && ((char *)lst->content)[i] != '\n')
		i++;
	while (((char *)lst->content)[i] && ((char *)lst->content)[++i])
		buf[j++] = ((char *)lst->content)[i];
	buf[j] = '\0';
	node->content = buf;
	node->next = NULL;
	safe_free(list, node);
}
