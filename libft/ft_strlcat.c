#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	len_src;
	size_t	len_dst;

	j = 0;
	len_src = ft_strlen(src);
	j = ft_strlen(dst);
	len_dst = j;
	if (dstsize == 0 && dst == 0)
		return (len_src);
	if (dstsize == 0 || dstsize <= len_dst)
		return (len_src + dstsize);
	i = 0;
	while (src[i] && i < dstsize - len_dst - 1)
	{
		dst[j] = src[i];
		i++;
		j++;
	}
	dst[j] = '\0';
	return (len_src + len_dst);
}
