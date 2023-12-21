/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/10 12:45:18 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/21 17:37:12 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_update_stash(char *stash)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	while (stash[i] && stash[i] != '\n')
		i++;
	if (!stash[i])
		return (NULL);
	str = ft_malloc(sizeof(char) * (ft_strlen2(stash) - i + 1), ALLOC);
	if (!str)
		return (NULL);
	i++;
	j = 0;
	while (stash[i])
		str[j++] = stash[i++];
	str[j] = '\0';
	return (str);
}

char	*ft_get_line(char *stash)
{
	int		i;
	char	*str;

	i = 0;
	if (!stash[i])
		return (NULL);
	while (stash[i] && stash[i] != '\n')
		i++;
	str = ft_malloc(sizeof(char) * (i + 2), ALLOC);
	if (!str)
		return (NULL);
	i = 0;
	while (stash[i] && stash[i] != '\n')
	{
		str[i] = stash[i];
		i++;
	}
	if (stash[i] == '\n')
	{
		str[i] = stash[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	*ft_read(int fd, char *stash)
{
	int		i;
	char	*buffer;

	buffer = ft_malloc(sizeof(char) * (BUFFER_SIZE + 1), ALLOC);
	if (!buffer)
		return (NULL);
	i = 1;
	while (i != 0 && !ft_is_newline(stash))
	{
		i = read(fd, buffer, BUFFER_SIZE);
		if (i == -1)
			return (NULL);
		buffer[i] = '\0';
		stash = ft_strjoin2(stash, buffer);
	}
	return (stash);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*stash[4096];

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (0);
	stash[fd] = ft_read(fd, stash[fd]);
	if (!stash[fd])
		return (NULL);
	line = ft_get_line(stash[fd]);
	stash[fd] = ft_update_stash(stash[fd]);
	return (line);
}
