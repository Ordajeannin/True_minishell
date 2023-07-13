/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_concerns.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 13:59:01 by asalic            #+#    #+#             */
/*   Updated: 2023/07/13 12:08:21 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Modif de l'env pour cd
 * Parcourt env_list jusqu'a l'element que l'on veut changer.
 * Puis, modifier cette valeur avec new_str
*/
void	change_env_cd(t_args **env_list, char *new_str, char *change_value)
{
	t_args	*current;

	current = *env_list;
	while (current)
	{
		if (ft_strncmp(current->str, change_value, ft_strlen(current->str))
			== 0)
		{
			current->str = NULL;
			current->str = new_str;
			return ;
		}
		current = current->next;
	}
}

/* 
 * Modif de env pour export.
 * Recherche de name_env dans env et modifs.
 * Boucle jusqu'a trouver name_env dans l'env.
 * Puis, modifie sa valeur avec value.
*/
int	change_env_exp(t_args **env_list, char *name_env, char *value)
{
	char	*result;
	char	*current_name;
	t_args	*current;

	result = ft_strjoin(name_env, "=");
	result = ft_strjoin(result, value);
	current_name = NULL;
	current = *env_list;
	while (current)
	{
		current_name = ft_strdupto_n(current->str, '=');
		if (ft_strncmp(current_name, name_env, ft_strlen(current_name)) == 0
			&& ft_strlen(current_name) == ft_strlen(name_env))
		{
			current->str = NULL;
			current->str = result;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

/* 
 * Boucle principale d'unset.
 * Cherche une VE et la supprime s'il la trouve.
*/
int	searchin_env(t_args **env_list, t_args *list)
{
	t_args	*current;
	t_args	*temp;
	char	*name_env;
	int		len;

	current = *env_list;
	len = ft_strlen(list->next->str);
	while (current && current->next)
	{
		name_env = ft_strdupto_n(current->next->str, '=');
		if (ft_strncmp(list->next->str, ft_strdupto_n(current->next->str, \
			'='), len) == 0 && len == ft_strlen(name_env))
		{
			temp = current->next->next;
			free(current->next);
			current->next = temp;
			return (1);
		}
		current = current->next;
	}
	return (0);
}

/* 
 * Fonction a l'image de 'env'
 * Affiche l'environnement du shell en entier
 * (Attention : env -i ./minishell doit afficher PWD, SHLVL et _)
*/
void	ft_env(t_args *list, t_args **env_list)
{
	t_args	*current;

	current = *env_list;
	if (list->next != NULL)
		ft_printf("bash: %s: %s: %s\n", list->str, list->next->str, \
			strerror(errno));
	while (current != NULL)
	{
		ft_printf("%s\n", current->str);
		current = current->next;
	}
}

/* 
 * Initialise liste d'env 
*/
int	set_env(t_args **env_list, char **env, t_shell *shell)
{
	int			i;
	t_args		*current;

	if (handle_env(env, shell) == -1)
		return (-1);
	i = 0;
	while (env[i])
	{
		add_arg(env_list, env[i], 0);
		i++;
	}
	i = 0;
	current = *env_list;
	while (env[i])
	{
		current->str = env[i++];
		current = current->next;
	}
	return (0);
}
