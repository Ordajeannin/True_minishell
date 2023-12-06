/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_env_concerns1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:15:44 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/06 18:00:29 by ajeannin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de norme change_env_exp
*/
int	help_cee(char **current_name, t_args **current)
{
	*current_name = ft_strdupto_n((*current)->str, '=');
	if (!*current_name)
		return (2);
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
		(*current)->str = ft_strdup(*result);
		return (0);
	}
	*current = (*current)->next;
	return (1);
}
