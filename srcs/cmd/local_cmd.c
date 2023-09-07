/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:13:22 by asalic            #+#    #+#             */
/*   Updated: 2023/09/07 15:33:34 by asalic           ###   ########.fr       */
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

/* 
 * Gere les boucles de export pour changer les VE et sinon les creer
*/
static void	ft_more_export(t_shell *shell, char *v_env, char *value)
{
	shell_change(shell, v_env, value);
	if (ft_strncmp(v_env, "PWD", ft_strlen(v_env)) == 0)
		shell->is_pwd = value;
	else if (ft_strncmp(v_env, "OLDPWD", ft_strlen(v_env)) == 0)
		shell->is_oldpwd = value;
}

/* Fonction export.
 * Cherche d'abord si la VE existe deja.
 * Si oui, la modifie, dans env_list et dans shell.
 * Si non, la creee dans env_list seulement.
 * Gere le cas ou il y a plusieurs creation/remplacement de VE
 * ATTENTION: if (list->next == NULL) : cas special
 * 			-->affiche declare -x et VE avec toutes les VE
*/
int	ft_export(t_args *list, t_shell *shell, t_args **env_list)
{
	char	*value;
	char	*v_env;

	v_env = ft_strdupto_n(list->next->str, '=');
	value = ft_strdup_from(list->next->str, '=');
	ft_printf("V_ENV = %s.\n", v_env);
	if (is_only_equal(value, '=') == 1 && (is_only_equal(v_env, ' ') == 1
			|| v_env == NULL))
		return (1);
	if (change_env_exp(env_list, v_env, value) == 1)
		ft_more_export(shell, v_env, value);
	else
	{
		add_env(env_list, list);
		ft_more_export(shell, v_env, value);
	}
	if (list->next->next != NULL)
		ft_export(list->next, shell, env_list);
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
	int	code_err;

	code_err = 0;
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
