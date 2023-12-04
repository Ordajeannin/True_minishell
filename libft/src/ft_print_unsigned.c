/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_unsigned.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/03 18:33:45 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/04 18:33:40 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_nbr_len(unsigned int nbr)
{
	int	len;

	len = 0;
	while (nbr != 0)
	{
		len++;
		nbr = nbr / 10;
	}
	return (len);
}

char	*ft_uitoa(unsigned int n)
{
	char	*nbr;
	int		len;

	len = ft_nbr_len(n);
	nbr = (char *)ft_malloc(sizeof(char) * (len + 1), ALLOC);
	if (!nbr)
		return (0);
	nbr[len] = '\0';
	while (n != 0)
	{
		nbr[--len] = n % 10 + '0';
		n = n / 10;
	}
	return (nbr);
}

int	ft_print_unsigned(unsigned int n)
{
	int		print_length;
	char	*nbr;

	print_length = 0;
	if (n == 0)
		print_length += write(1, "0", 1);
	else
	{
		nbr = ft_uitoa(n);
		print_length += ft_print_str(nbr);
		// free(nbr);
	}
	return (print_length);
}
