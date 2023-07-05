/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 12:37:52 by asalic            #+#    #+#             */
/*   Updated: 2023/07/05 10:55:28 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_echo(char *args)
{
	int		len_tab;
	int		i;
	int		codon;

	i = 5;
	len_tab = 0;
	codon = 0;
	while (args[i++])
	{
		if (args[i] == 34 && codon == 0)
			codon = 1;
		if (args[i] == 39 && codon == 0)
			codon = 2;
		if ((args[i] == 39 && codon == 2) || (args[i] == 34 && codon == 1))
		{
			len_tab ++;
			codon = 0;
		}
	}
	if (codon == 1 || codon == 2)
		return (-1);
	return (len_tab);
}

int	send_toecho(int len_tab, char *args, int i)
{
	char	**new_tab;
	int		i_tab;

	i_tab = 0;
	new_tab = malloc(1 * sizeof(char *));
	if (!new_tab)
		exit (EXIT_FAILURE);
	new_tab[0] = malloc((len_tab +1) * sizeof(char));
	if (!new_tab[0])
		exit (EXIT_FAILURE);
	i ++;
	while (args[i] && args[i] != 34 && args[i] != 39)
	{
		new_tab[0][i_tab] = args[i];
		i ++;
		i_tab++;
	}
	new_tab[0][i_tab++] = '\0';
	ft_echo(new_tab[0]);
	free(new_tab[0]);
	free(new_tab);
	i ++;
	return (i);
}

void	args_echo(char *args)
{
	int		len_tab;
	int		i;

	if (!args[5])
	{
		write (1, "\n", 1);
		return ;
	}
	if (parse_echo(args) == -1)
		return ;
	len_tab = parse_echo(args);
	i = 5;
	while (args[i])
	{
		if (args[i] == 34 || args[i] == 39)
		{
			i += send_toecho(len_tab, args, i);
		}
		i ++;
	}
	write (1, "\n", 1);
}

void	ft_echo(char *args)
{
	int	i;

	i = 0;
	while (args[i])
		write (1, &args[i++], 1);
}
