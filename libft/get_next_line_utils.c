#include "libft.h"

int	found_new_line(t_list *list)
{
	int	i;

	while (list)
	{
		i = 0;
		while (((char *)list->content)[i])
		{
			if ('\n' == ((char *)list->content)[i])
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
		while (((char *)list->content)[i])
		{
			if ('\n' == ((char *)list->content)[i])
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
	ft_lstclear(list, free);
	if (((char *)node->content)[0])
		*list = node;
	else
	{
		free(node->content);
		free(node);
	}
}
