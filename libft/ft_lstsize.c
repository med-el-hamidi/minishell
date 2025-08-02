#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	size_t	count;
	t_list	*ptr;

	count = 0;
	ptr = lst;
	while (ptr != NULL)
	{
		count++;
		ptr = ptr->next;
	}
	return (count);
}
