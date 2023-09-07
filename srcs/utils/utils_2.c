/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 09:31:12 by asalic            #+#    #+#             */
/*   Updated: 2023/09/07 15:56:11 by asalic           ###   ########.fr       */
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
