/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_env_concerns1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:15:44 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/04 18:20:03 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de norme change_env_exp
*/
int	help_cee(char **current_name, t_args **current, char **result)
{
	*current_name = ft_strdupto_n((*current)->str, '=');
	// if (!*current_name)
	// {
	// 	free(*result);
	// 	return (2);
	// }
	return (0);
}

/*
 * Permet de norme change_env_exp
*/
int	help_cee2(char **current_name, t_args **current, char **result,
		char **name_env)
{
	if (ft_strncmp(*current_name, *name_env, ft_strlen(*current_name)) == 0
		&& ft_strlen(*current_name) == ft_strlen(*name_env))
	{
		// if ((*current)->str)
		// 	free((*current)->str);
		(*current)->str = ft_strdup(*result);
		// free(*current_name);
		// free(*result);
		return (0);
	}
	*current = (*current)->next;
	// free(*current_name);
	return (1);
}

/*
 * Permet de norme change_env_exp
*/
int	help_cee3(char **result)
{
	// free(*result);
	return (1);
}
