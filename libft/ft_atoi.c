#include "libft.h"

int	ft_atoi(const char *str)
{
	long	nbr;
	long	tmp;
	int		sign;

	nbr = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == ' ')
		str++;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		tmp = nbr;
		nbr = (nbr * 10) + (*str++ - '0');
		if ((nbr / 10) != tmp && sign == 1)
			return (-1);
		else if ((nbr / 10) != tmp && sign == -1)
			return (0);
	}
	return (nbr * sign);
}
