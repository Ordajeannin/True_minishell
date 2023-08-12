/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/11 12:12:28 by asalic            #+#    #+#             */
/*   Updated: 2023/08/12 15:18:08 by asalic           ###   ########.fr       */
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
	else if (ft_strncmp(str, "OLDPWD", len) == 0)
		shell->oldpwd = value;
	else if (ft_strncmp(str, "PWD", len) == 0)
		shell->pwd = value;
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
void	add_env(t_args **env_list, t_args *list)
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

int	len_targs(t_args *list)
{
	int	len;

	len = 0;
	while (list)
	{
		len ++;
		list = list->next;
	}
	return (len);
}

char	**dup_double_string(t_args **e_list)
{
	char	**result;
	t_args	*current;
	int		i;

	current = *e_list;
	result = ft_calloc(len_targs(current), sizeof(char *));
	if (result == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (current)
	{
		result[i] = ft_strdup(current->str);
		if (result[i] == NULL)
		{
			while (i >= 0)
				free(result[i--]);
			free(result);
			exit(EXIT_FAILURE);
		}
		i ++;
		current = current->next;
	}
	return (result);
}
