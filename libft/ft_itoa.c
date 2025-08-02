#include "libft.h"

static unsigned int	ft_nbrlen(int n)
{
	unsigned int	i;

	i = 0;
	if (n <= 0)
		i = 1;
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

static void	ft_itoa_recursive(int n, char *res, unsigned int *i)
{
	if (n <= -10 || n >= 10)
		ft_itoa_recursive(n / 10, res, i);
	if (n < 0)
		res[(*i)++] = -(n % 10) + '0';
	else
		res[(*i)++] = (n % 10) + '0';
}

char	*ft_itoa(int n)
{
	char			*res;
	unsigned int	len;
	unsigned int	i;

	len = ft_nbrlen(n);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	i = 0;
	if (n < 0)
		res[i++] = '-';
	ft_itoa_recursive(n, res, &i);
	res[i] = '\0';
	return (res);
}
