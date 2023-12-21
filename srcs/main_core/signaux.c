/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:32:05 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/21 18:29:47 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 *gestionnaire de signal pou le heredoc
 *
*/
void	signal_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit (130);
}

/* 
 * Gestionnaire de signaux
 * Renvoie l'invite de commande lorsque sig == CTRL-C
 */
void	signal_handler(int sig)
{
	if (rl_done && g_error == 2)
	{
		ft_printf("\n");
		set_error_nb(130, YES);
	}
	if (!rl_done)
	{
		set_error_nb(130, YES);
		ft_printf("\n");
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
	else if (sig == 3)
	{
		ft_printf("Quit (core dumped)\n");
		set_error_nb(131, YES);
	}
	return ;
}
