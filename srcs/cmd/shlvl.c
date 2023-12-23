/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shlvl.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/04 17:03:17 by asalic            #+#    #+#             */
/*   Updated: 2023/12/23 14:48:48 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 Evolution de ft_atoi
 Cherche dans une string un enchainement de nombre et le transforme en nombre
*/
static unsigned int	ft_atoi_evolve(char	*str)
{
	unsigned int	i;
	unsigned int	result;

	i = 0;
	while (str[i] < '0' || str[i] > '9')
		i++;
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
		result = (result * 10) + (str[i++] - '0');
	return (result);
}

/* 
 Incremente la VE SHLVL
 Lorsque un ./minishell est lance dans un ./minishell -> SHLVL +1
*/
int	ft_plus_shell(t_shell *shell, t_args **env_list)
{
	unsigned int	nb_shell;
	char			*len_shell;

	nb_shell = ft_atoi_evolve(shell->shlvl) + 1;
	len_shell = ft_itoa(nb_shell);
	if (!len_shell)
		return (1);
	shell->shlvl = ft_strdup(len_shell);
	if (!shell->shlvl)
		return (1);
	if (change_env_exp(env_list, "SHLVL", len_shell) == 2)
		return (1);
	return (0);
}
