#include "libft.h"

static size_t	ft_cwords(char const *s, char c)
{
	size_t	words;
	int		flag;

	words = 0;
	flag = 0;
	while (*s)
	{
		if (*s != c && !flag)
		{
			flag = 1;
			words++;
		}
		else if (*s == c)
			flag = 0;
		s++;
	}
	return (words);
}

static size_t	ft_sublen(char const *s, char c)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != c)
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

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	i;

	if (!s)
		return (NULL);
	res = (char **)malloc(sizeof(char *) * (ft_cwords(s, c) + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (*s)
	{
		if (*s != c)
		{
			res[i] = (char *)malloc(sizeof(char) * (ft_sublen(s, c) + 1));
			if (!res[i])
				return (ft_free(res, i), NULL);
			ft_strlcpy(res[i], s, (ft_sublen(s, c) + 1));
			i++;
			s += ft_sublen(s, c);
		}
		else
			s++;
	}
	res[i] = NULL;
	return (res);
}
