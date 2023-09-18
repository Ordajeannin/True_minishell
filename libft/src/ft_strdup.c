/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:51:02 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/18 13:17:52 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	int		i;
	char	*dup;

	if (!s)
		return (ft_strdup(""));
	i = ft_strlen(s) + 1;
	dup = malloc(sizeof(char) * i);
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, s, i);
	return (dup);
}
