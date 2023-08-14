/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:13:22 by asalic            #+#    #+#             */
/*   Updated: 2023/08/14 17:48:03 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Affiche le repertoire courrant 
 * Fonction a l'image de 'pwd'
*/
int	ft_pwd(t_shell *shell)
{
	if (shell->is_pwd == NULL)
	{
		shell->error = errno;
		return (1);
	}
	else
		ft_printf("%s\n", shell->is_pwd);
	shell->error = 0;
	return (0);
}

/* 
 * Fonction unset.
 * Supprime une variable d'environnement appele.
 * Change aussi les VE saved dans struct t_shell a NULL.
 * Si elle est vide ou n'existe pas, renvoie juste l'invite de commande.
*/
int	ft_unset(t_args *list, t_shell *shell, t_args *env_list)
{
	if (!list)
		return (1);
	if (!searchin_env(&env_list, list))
		return (1);
	else
		shell_change(shell, list->next->str, NULL);
	if (list->next->next != NULL)
		ft_unset(list->next, shell, env_list);
	shell->error = 0;
	return (0);
}

/* Fonction export.
 * Cherche d'abord si la VE existe deja.
 * Si oui, la modifie, dans env_list et dans shell.
 * Si non, la creee dans env_list seulement.
*/
int	ft_export(t_args *list, t_shell *shell, t_args **env_list)
{
	char	*value;
	char	*v_env;

	v_env = ft_strdupto_n(list->next->str, '=');
	value = ft_strdup_from(list->next->str, '=');
	if (change_env_exp(env_list, v_env, value) == 1)
	{
		shell_change(shell, v_env, value);
		if (ft_strncmp(v_env, "PWD", ft_strlen(v_env)) == 0)
			shell->is_pwd = value;
		else if (ft_strncmp(v_env, "OLDPWD", ft_strlen(v_env)) == 0)
			shell->is_oldpwd = value;
	}
	else
	{
		add_env(env_list, list);
		shell_change(shell, v_env, value);
		if (ft_strncmp(v_env, "PWD", ft_strlen(v_env)) == 0)
			shell->is_pwd = value;
		else if (ft_strncmp(v_env, "OLDPWD", ft_strlen(v_env)) == 0)
			shell->is_oldpwd = value;
	}
	shell->error = 0;
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
	free(input);
	clear_args_list(&list);
	clear_args_list(&env_list);
	rl_clear_history();
	ft_printf("exit\n");
	exit(EXIT_FAILURE);
}
