/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 09:31:12 by asalic            #+#    #+#             */
/*   Updated: 2023/09/27 14:11:26 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Compte le nombre de directory qui n'existe plus depuis rm -rf.
 * Ignore les /..
*/
int	count_dir(t_shell *shell)
{
	int		len;
	char	*temp;
	DIR		*dir;

	len = 0;
	temp = shell->is_pwd;
	dir = opendir(temp);
	while (!dir)
	{
		if (temp[ft_strlen(temp) - 1] != '.' && temp[ft_strlen(temp) - 2] \
			!= '.' && temp[ft_strlen(temp) - 3] != '/')
			len ++;
		temp = from_end_to_char(temp, '/');
		closedir(dir);
		dir = opendir(temp);
	}
	closedir(dir);
	return (len -1);
}

/*
 * Compte le nombre de /.. apres les directory.
*/
int	count_back(char	*str)
{
	int		len;
	int		i;
	char	**tab;

	tab = ft_split(str, '/');
	i = ft_strlen_double(tab) - 1;
	len = 0;
	while (i >= 0)
	{
		if (ft_strncmp(tab[i], "..", ft_strlen(tab[i])) == 0)
			len ++;
		else
			return (len);
		i --;
	}
	return (len);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	size_t	i;
	size_t	j;
	size_t	l;
	char	*new;

	i = 0;
	j = 0;
	if (!s1 && !s2)
		return (NULL);
	if (!s1)
		return ((char *)s2);
	if (!s2)
		return ((char *)s1);
	l = ft_strlen(s1) + ft_strlen(s2);
	new = ft_calloc(l + 1, sizeof(char));
	if (!new)
		return (NULL);
	while (s1[i])
		new[j++] = s1[i++];
	i = 0;
	while (s2[i])
		new[j++] = s2[i++];
	new[j] = '\0';
	return (free(s1), free(s2), new);
}
