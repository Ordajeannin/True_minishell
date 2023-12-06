/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 16:44:59 by asalic            #+#    #+#             */
/*   Updated: 2023/12/06 16:36:07 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet a main d'etre a moins de 25 lines
 * Fonction purement utilitaire, ne pas garder dans le rendu final
*/
void	ft_gain_place(char **av, t_args **list,
		t_args **env_list)
{
	g_error = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	*list = NULL;
	*env_list = NULL;
	(void)av;
}
