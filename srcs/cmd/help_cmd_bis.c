/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_cmd_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 08:42:25 by asalic            #+#    #+#             */
/*   Updated: 2023/09/18 08:46:57 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Fonction d'extension de cd.
 * Permets de mettre a jour ou d'export pwd et oldpwd lorsque cd change
 * de repertoire.
*/
void	cd_move_and_change(t_args *env_list, t_shell *shell)
{
	change_env_cd(&env_list, ft_strjoin("OLDPWD=", shell->is_pwd),
		ft_strjoin("OLDPWD=", shell->is_oldpwd));
	shell->is_oldpwd = shell->is_pwd;
	shell->oldpwd = shell->is_pwd;
	if (getcwd(NULL, 0) != NULL)
	{
		change_env_cd(&env_list, ft_strjoin("PWD=", getcwd(NULL, 0)),
			ft_strjoin("PWD=", shell->is_pwd));
		shell->is_pwd = getcwd(NULL, 0);
		shell->pwd = getcwd(NULL, 0);
	}
}
