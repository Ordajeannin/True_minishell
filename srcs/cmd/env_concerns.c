/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_concerns.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/07 13:59:01 by asalic            #+#    #+#             */
/*   Updated: 2023/10/03 12:20:28 by asalic           ###   ########.fr       */
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
	while (current && current->str != NULL)
	{
		if (ft_strcmp(current->str, change_value) == 0 && \
			ft_strlen(current->str) == ft_strlen(change_value))
		{
			current->str = ft_strdup(new_str);
			return ;
		}
		current = current->next;
		if (current == NULL)
			add_env(env_list, new_str);
	}
}

/* 
 * Modif de env pour export
 * Recherche de name_env dans env et modifs.
 * Boucle jusqu'a trouver name_env dans l'env.
 * Puis, modifie sa valeur avec value.
*/
int	change_env_exp(t_args **env_list, char *name_env, char *value)
{
	char	*result;
	char	*tmp;
	char	*current_name;
	t_args	*current;

	tmp = ft_strjoin(name_env, "=");
	if (! tmp)
		return (2);
	if (ft_strcmp(name_env, "SHLVL") == 0 && ft_strlen((const char *)name_env) \
		== 5 && ft_atoi(value) < 0)
		result = ft_strjoin(tmp, "0");
	else
		result = ft_strjoin(tmp, value);
	free(tmp);
	if (!result)
		return (2);
	current = *env_list;
	while (current && current->next != NULL)
	{
		current_name = ft_strdupto_n(current->str, '=');
		if (!current_name)
		{
			free(result);
			return (2);
		}
		if (ft_strncmp(current_name, name_env, ft_strlen(current_name)) == 0
			&& ft_strlen(current_name) == ft_strlen(name_env))
		{
			current->str = ft_strdup(result);
			free(current_name);
			free(result);
			return (0);
		}
		current = current->next;
		free(current_name);
	}
	free(result);
	return (1);
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
	size_t	len;

	current = *env_list;
	len = ft_strlen(list->next->str);
	while (current && current->next)
	{
		name_env = ft_strdupto_n(current->next->str, '=');
		if (!name_env)
			return (1);
		if (ft_strncmp(list->next->str, name_env, len) == 0 && \
		len == ft_strlen(name_env))
		{
			temp = current->next->next;
			free(current->next);
			current->next = temp;
			free(name_env);
			return (0);
		}
		free(name_env);
		current = current->next;
	}
	return (1);
}

/* 
 * Fonction a l'image de 'env'
 * Affiche l'environnement du shell en entier
 * (Attention : env -i ./minishell doit afficher PWD, SHLVL et _)
*/
int	ft_env(t_args *list, t_args **env_list, t_shell *shell)
{
	t_args	*current;

	update_last_ve(list, env_list);
	current = *env_list;
	if (list->next != NULL)
		return (1);
	while (current != NULL && current->str != NULL)
	{
		if (ft_strncmp(current->str, "?=", 2) == 0)
			current = current->next;
		else
		{
			ft_printf("%s\n", current->str);
			current = current->next;
		}
	}
	if (!change_error(env_list, shell, 0))
		return (1);
	return (0);
}

/* 
 * Initialise liste d'env 
*/
int	set_env(t_args **env_list, char **env, t_shell *shell)
{
	int		i;
	char	*identifier;

	if (*env == NULL)
	{
		set_empty_env(shell, env_list);
		return (0);
	}
	if (handle_env(env, shell) == -1)
		return (-1);
	i = 0;
	while (env[i])
	{
		add_arg(env_list, env[i], 0);
		identifier = ft_strdupto_n(env[i], '=');
		if (! identifier)
			return (-1);
		if (ft_strcmp(identifier, "SHLVL") == 0 && \
		ft_strlen(identifier) == 5)
		{
			if (ft_plus_shell(shell, env_list) == 1)
				return (-1);
		}
		free(identifier);
		i++;
	}
	add_arg(env_list, "?=0", 0);
	return (0);
}
