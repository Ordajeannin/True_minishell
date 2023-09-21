/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ajeannin <ajeannin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/21 17:32:05 by ajeannin          #+#    #+#             */
/*   Updated: 2023/09/21 17:32:13 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* 
 * Gestionnaire de signaux
 * Renvoie l'invite de commande lorsque sig == CTRL-C
 */
void	signal_handler(int sig)
{
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
