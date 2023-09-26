/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/26 16:44:59 by asalic            #+#    #+#             */
/*   Updated: 2023/09/26 16:46:19 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet a main d'etre a moins de 25 lines
 * Fonction purement utilitaire, ne pas garder dans le rendu final
*/
void	ft_gain_place(char **av, t_args **list, char **input,
		t_args **env_list)
{
	g_error = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
	*list = NULL;
	*input = NULL;
	*env_list = NULL;
	(void)av;
}
