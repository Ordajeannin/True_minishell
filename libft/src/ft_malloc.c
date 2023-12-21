/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 14:37:34 by pkorsako          #+#    #+#             */
/*   Updated: 2023/12/21 20:27:53 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	free_garbage(t_garbage *alloc_elements)
{
	t_garbage	*index;

	alloc_elements = alloc_elements->next;
	index = alloc_elements;
	while (alloc_elements)
	{
		index = alloc_elements;
		free(alloc_elements->element);
		alloc_elements = alloc_elements->next;
		free(index);
	}
}

/*
 * ATTENTION VALUE EXIT
*/
void	upgrade_list(t_garbage *alloc_elements, void *allocated_element)
{
	t_garbage	*new_struct;
	t_garbage	*index;

	new_struct = malloc(sizeof(t_garbage));
	if (!new_struct)
	{
		ft_malloc(0, FREE);
		printf("alloc error\n");
		exit(1);
	}
	index = alloc_elements;
	while (index->next)
		index = index->next;
	index->next = new_struct;
	new_struct->element = allocated_element;
	new_struct->next = NULL;
}

void	*ft_malloc(size_t byte_size, int action)
{
	static t_garbage	alloc_elements;
	void				*allocated_element;

	if (action == FREE)
		free_garbage(&alloc_elements);
	else
	{
		allocated_element = malloc(byte_size);
		if (!allocated_element)
		{
			ft_malloc(0, FREE);
			printf("alloc error\n");
			exit (1);
		}
		upgrade_list(&alloc_elements, allocated_element);
		return (allocated_element);
	}
	return (NULL);
}
