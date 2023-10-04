/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:32:05 by ajeannin          #+#    #+#             */
/*   Updated: 2023/10/04 19:17:21 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Gestionnaire de signaux
 * Renvoie l'invite de commande lorsque sig == CTRL-C
 */
void	signal_handler(int sig)
{
	ft_printf("je suis la\n");
	// ft_printf("sig vaut : %d\n", sig);
	if (rl_done && g_error == 2)
	{
		ft_printf("\n");
		code_error(130);
	}
	if (!rl_done)
	{
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		code_error(130);
	}
	else if (sig == 3)
	{
		ft_printf("Quit (core dumped)\n");
		code_error(131);
	}
	return ;
}
