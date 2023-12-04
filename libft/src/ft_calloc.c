/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/08 17:26:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/04 17:38:05 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*dst;

	if (count == SIZE_MAX && size == SIZE_MAX)
		return (0);
	dst = (void *)ft_malloc(count * size, ALLOC);
	if (!dst)
		return (0);
	ft_bzero(dst, count * size);
	return (dst);
}
