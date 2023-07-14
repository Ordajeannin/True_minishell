/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 12:12:28 by asalic            #+#    #+#             */
/*   Updated: 2023/07/13 17:28:21 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Changement des valeurs de la structure t_shell.
 * S'effectue apres unset (et export aussi !!)
*/
void	shell_change(t_shell *shell, char *str, char *value)
{
	int		len;

	len = ft_strlen(str);
	if (ft_strncmp(str, "HOME", len) == 0)
		shell->home = value;
	else if (ft_strncmp(str, "PWD", len) == 0)
		shell->pwd = value;
	else if (ft_strncmp(str, "OLDPWD", len) == 0)
		shell->oldpwd = value;
	else if (ft_strncmp(str, "USER", len) == 0)
		shell->user = value;
	else if (ft_strncmp(str, "SHELL", len) == 0)
		shell->shell = value;
	else if (ft_strncmp(str, "PATH", len) == 0)
		shell->path = value;
	else if (ft_strncmp(str, "LANG", len) == 0)
		shell->lang = value;
	else if (ft_strncmp(str, "TERM", len) == 0)
		shell->term = value;
	else if (ft_strncmp(str, "HOSTNAME", len) == 0)
		shell->hostname = value;
	else if (ft_strncmp(str, "SHLVL", len) == 0)
		shell->shlvl = value;
	return ;
}

/*
 * Ajout nouveau maillon dans env.
 * Boucle jusqu'a la fin de la liste.
 * Puis ajoute la string.
 * (BUG: sauf qu'il l'ajoute de maniere random ou il veut dans l'env ;-;)
*/
static void	add_env(t_args **env_list, t_args *list)
{
	t_args	*new_var;
	t_args	*current;

	new_var = malloc(sizeof(t_args));
	new_var->str = ft_strdup(list->next->str);
	new_var->token = 0;
	new_var->next = NULL;
	if (*env_list)
	{
		current = *env_list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_var;
	}
	else
		*env_list = new_var;
}

/* Fonction export.
 * Cherche d'abord si la VE existe deja.
 * Si oui, la modifie, dans env_list et dans shell.
 * Si non, la creee dans env_list seulement.
*/
void	ft_export(t_args *list, t_shell *shell, t_args **env_list)
{
	char	*value;
	char	*v_env;

	v_env = ft_strdupto_n(list->next->str, '=');
	value = ft_strdup_from(list->next->str, '=');
	if (change_env_exp(env_list, v_env, value) == 1)
		shell_change(shell, v_env, value);
	else
	{
		add_env(env_list, list);
		shell_change(shell, v_env, value);
	}
	shell->error = 0;
}

/*
 * Program stop.
 * Fonction a l'image de 'exit' 
 * Quitte le programme proprement avec free.
 * :warning:
 * wait(100) supprime de la premiere ligne pour la norme
*/
void	ft_exit(char *input, t_args *list, t_args *env_list)
{
	free(input);
	clear_args_list(&list);
	clear_args_list(&env_list);
	ft_printf("exit\n");
	exit(EXIT_FAILURE);
}
