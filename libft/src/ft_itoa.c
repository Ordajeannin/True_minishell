/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/09 02:31:53 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/26 15:57:58 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_intlen(int n)
{
	int	len;

	len = 0;
	if (n < 0)
	{
		len = 1;
		n = -n;
	}
	if (n == 0)
		len = 1;
	while (n != 0)
	{
		n = n / 10;
		len ++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*nbr;
	int		i;
	int		len_n;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	len_n = ft_intlen(n);
	nbr = malloc((len_n + 1) * sizeof(char));
	if (!nbr)
		return (NULL);
	i = 1;
	if (n < 0)
	{
		nbr[0] = '-';
		n = -n;
	}
	nbr[len_n] = '\0';
	while (n > 9)
	{
		nbr[len_n - i] = (n % 10) + 48;
		n = n / 10;
		i++;
	}
	nbr[len_n - i] = n + 48;
	return (nbr);
}

// static	char	*ft_write_number(int len, int n, char *a)
// {
// 	while (len >= 0)
// 	{
// 		a[len] = '0' + (n % 10);
// 		n = n / 10;
// 		len --;
// 	}
// 	return (a);
// }

// static	int	ft_len(int n)
// {
// 	int	len;

// 	if (n <= 0)
// 	{
// 		len = 1;
// 		n = -n;
// 	}
// 	else
// 		len = 0;
// 	while (n > 0)
// 	{
// 		n = n / 10;
// 		len++;
// 	}
// 	return (len);
// }

// static	char	*ft_minitoa(int n)
// {
// 	char	*a;

// 	n = n - 1;
// 	a = (char *)malloc(sizeof(char) * 12);
// 	a[0] = '-';
// 	a[1] = '2';
// 	a[2] = '1';
// 	a[3] = '4';
// 	a[4] = '7';
// 	a[5] = '4';
// 	a[6] = '8';
// 	a[7] = '3';
// 	a[8] = '6';
// 	a[9] = '4';
// 	a[10] = '8';
// 	a[11] = '\0';
// 	return (a);
// }

// char	*ft_itoa(int n)
// {
// 	int		len;
// 	int		sign;
// 	char	*a;

// 	ft_printf("%d\n", n);
// 	len = ft_len(n);
// 	if (n == -2147483648)
// 		return (ft_minitoa(n));
// 	if (n < 0 && n != -2147483648)
// 	{
// 		sign = -1;
// 		n = -n;
// 	}
// 	else if (n != -2147483648)
// 		sign = 1;
// 	a = ft_calloc(len +1, sizeof(char));
// 	if (!a)
// 		return (NULL);
// 	a[len] = '\0';
// 	len--;
// 	ft_write_number(len, n, a);
// 	if (sign == -1)
// 		a[0] = '-';
// 	return (a);
// }
