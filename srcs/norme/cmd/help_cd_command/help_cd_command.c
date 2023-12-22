/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_cd_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 13:10:59 by ajeannin          #+#    #+#             */
/*   Updated: 2023/12/19 15:49:10 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Permet de normer is_two_point
*/
char	*help_itp1(DIR **dir)
{
	ft_printf("cd : No such file of directory\n");
	set_error_nb(0, YES);
	closedir(*dir);
	return (NULL);
}

/*
 * Permet de normer is_two_points
 * fonction recurrente dans le code
*/
char	*help_itp2(DIR **dir)
{
	closedir(*dir);
	return (NULL);
}
