/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/28 15:09:25 by asalic            #+#    #+#             */
/*   Updated: 2023/12/05 18:29:00 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Program stop.
 * Fonction a l'image de 'exit'
 * Quitte le programme proprement avec free.
 * :warning:
 * wait(100) supprime de la premiere ligne pour la norme
*/
int	ft_exit(t_args *list, t_args *env_list, t_shell *shell)
{
	int	code_err;

	code_err = 0;
	(void)shell;
	ft_printf("exit\n");
	if (list && list->next && is_numeric(list->next->str) == 1)
	{
		ft_printf("%s: %s: numeric argument required\n", list->str,
			list->next->str);
		code_err = 2;
	}
	else if (list && list->next && list->next->next)
	{
		ft_printf("%s: too many arguments\n", list->str);
		change_error(&env_list, shell, 1);
		return (1);
	}
	else if (list && list->next)
		code_err = ft_atoi(list->next->str) % 256;
	else if (shell->error == 0 && g_error != 0)
		code_err = g_error;
	else
		code_err = shell->error;
	ft_malloc(0, FREE);
	rl_clear_history();
	exit(code_err);
}
