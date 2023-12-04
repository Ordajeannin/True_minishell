/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_env_concerns2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:18:23 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/04 18:20:22 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de norme searchin_env
*/
int	help_s_e(t_args **temp, t_args **current, char **name_env)
{
	*temp = (*current)->next->next;
	// free((*current)->next);
	(*current)->next = *temp;
	// free(*name_env);
	return (0);
}

/*
 * Permet de norme set_env
*/
int	help_set_env(t_args **env_list, char **env, int *i,
		char **identifier)
{
	add_env(env_list, env[*i]);
	*identifier = ft_strdupto_n(env[*i], '=');
	if (!*identifier)
		return (-1);
	return (0);
}

/*
 * Permet de norme set_env
*/
int	help_set_env2(t_args **env_list, t_shell **shell, char **identifier,
		int *i)
{
	if (ft_strcmp(*identifier, "SHLVL") == 0
		&& ft_strlen(*identifier) == 5)
	{
		if (ft_plus_shell(*shell, env_list) == 1)
			return (-1);
	}
	// free(*identifier);
	(*i)++;
	return (0);
}
