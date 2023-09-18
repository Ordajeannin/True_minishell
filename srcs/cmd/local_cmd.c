/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:13:22 by asalic            #+#    #+#             */
/*   Updated: 2023/09/18 18:52:34 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Affiche le repertoire courrant 
 * Fonction a l'image de 'pwd'
*/
int	ft_pwd(t_shell *shell, t_args **env_list)
{
	if (shell->is_pwd == NULL)
	{
		change_error(env_list, errno);
		return (1);
	}
	else
		ft_printf("%s\n", shell->is_pwd);
	change_error(env_list, 0);
	return (0);
}

/* 
 * Fonction unset.
 * Supprime une variable d'environnement appelee.
 * Change aussi les VE saved dans struct t_shell a NULL.
 * Si elle est vide ou n'existe pas, renvoie juste l'invite de commande.
*/
int	ft_unset(t_args *list, t_shell *shell, t_args *env_list)
{
	if (!list->next)
		return (1);
	if (searchin_env(&env_list, list))
		shell_change(shell, list->next->str, NULL);
	if (list->next->next != NULL)
		ft_unset(list->next, shell, env_list);
	change_error(&env_list, 0);
	return (0);
}

/*
 * Program stop.
 * Fonction a l'image de 'exit' 
 * Quitte le programme proprement avec free.
 * :warning:
 * wait(100) supprime de la premiere ligne pour la norme
*/
int	ft_exit(char *input, t_args *list, t_args *env_list, t_shell *shell)
{
	int	code_err;

	code_err = 0;
	if (list && list->next && list->next->next)
	{
		ft_printf("%s: too many arguments\n", list->str);
		return (change_error(&env_list, 1));
	}
	free(input);
	clear_args_list(&env_list);
	rl_clear_history();
	ft_printf("exit\n");
	if (list && list->next != NULL)
	{
		if (is_numeric(list->next->str) == 1)
		{
			ft_printf("%s: %s: numeric argument required\n", list->str,
				list->next->str);
			code_err = 2;
		}
		else
			code_err = ft_atoi(list->next->str) % 256;
	}
	clear_args_list(&list);
	exit(code_err);
}
