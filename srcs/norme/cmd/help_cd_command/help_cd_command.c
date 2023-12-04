/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_cd_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:10:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/04 18:19:35 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de normer is_two_point
*/
char	*help_itp1(t_args *env_list, t_shell **shell, DIR **dir, char **temp)
{
	ft_printf("cd : No such file of directory\n");
	change_error(&env_list, *shell, 0);
	closedir(*dir);
	// free(*temp);
	return (NULL);
}

/*
 * Permet de normer is_two_points
 * fonction recurrente dans le code
*/
char	*help_itp2(DIR **dir, char **temp)
{
	// free(*temp);
	closedir(*dir);
	return (NULL);
}
