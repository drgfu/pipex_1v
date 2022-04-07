/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   substr_dup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkais <kkais@student.42wolfsburg.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 13:40:45 by kkais             #+#    #+#             */
/*   Updated: 2022/04/07 13:41:58 by kkais            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	ft_len(const char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_strdup(const char *str)
{
	char	*res;
	int		len;
	int		i;

	res = NULL;
	i = 0;
	len = ft_len(str);
	res = (char *)malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	while (str[i])
	{
		res[i] = str[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*if_star(void)
{
	char	*ptr;

	ptr = NULL;
	ptr = (void *)malloc(sizeof(char));
	if (!ptr)
		return (0);
	ptr[0] = '\0';
	return (ptr);
}

size_t	new_lenght(char *str, size_t len, unsigned int new_start)
{
	size_t	i;

	i = 0;
	while (str[new_start] && i < len)
	{
		i++;
		new_start++;
	}
	return (i);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char			*ptr;
	char			*str;
	size_t			i;
	unsigned int	new_start;

	i = 0;
	ptr = NULL;
	if (!s)
		return (NULL);
	str = (char *) s;
	new_start = start;
	if (start > ft_strlen(s))
		return (if_star());
	ptr = (void *)malloc((new_lenght(str, len, new_start) + 1) * sizeof(char));
	if (!ptr)
		return (0);
	while ((i < len) && (str[start] != '\0'))
		ptr[i++] = str[start++];
	ptr[i] = '\0';
	return (ptr);
}
