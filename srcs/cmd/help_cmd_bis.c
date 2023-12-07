/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help_cmd_bis.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pkorsako <pkorsako@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 08:42:25 by asalic            #+#    #+#             */
/*   Updated: 2023/12/07 19:35:27 by pkorsako         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Fonction d'extension de cd.
 * Permets de mettre a jour ou d'export pwd et oldpwd lorsque cd change
 * de repertoire.
*/

// int	cd_move_and_change(t_args *env_list, t_shell *shell)
// {
// 	char	*old_pwd_change;
// 	char	*old_cmd;
// 	char	*current_cmd;
// 	char	*new_pwd;
// 	char	*tmp;

// 	old_pwd_change = ft_strjoin("OLDPWD=", shell->is_pwd);
// 	if (!old_pwd_change)
// 		return (1);
// 	old_cmd = ft_strjoin("OLDPWD=", shell->is_oldpwd);
// 	if (!old_cmd)
// 		return (1);
// 	change_env_cd(&env_list, old_pwd_change, old_cmd);
// 	shell->is_oldpwd = ft_strdup(shell->is_pwd);
// 	shell->oldpwd = ft_strdup(shell->is_pwd);
// 	tmp = getcwd(NULL, 0);
// 	current_cmd = ft_strdup(tmp);
// 	if (current_cmd != NULL)
// 	{
// 		new_pwd = ft_strjoin("PWD=", current_cmd);
// 		 if (!new_pwd)
// 		 	return (1);
// 		old_cmd = ft_strjoin("PWD=", shell->is_pwd);
// 		 if (!old_cmd)
// 		 	return (1);
// 		change_env_cd(&env_list, new_pwd, old_cmd);
// 		shell->is_pwd = ft_strdup(current_cmd);
// 		shell->pwd = ft_strdup(current_cmd);
// 	}
// 	else
// 		return (1);
// 	return (0);
// }

////////////////////////////////PAUL version (mieux)///////////////////////

void	update_pwd(t_args *env_list, t_shell *shell)//met a jours PWD et OLDPWD dans l'env et secret_pwd
{
	t_args	*PWD;
	t_args	*OLDPWD;
	
	PWD = find_a("PWD", env_list);
	OLDPWD = find_a("OLDPWD", env_list);
	if (PWD)
		PWD->str = ft_strjoin("PWD=", ft_getcwd());
	if (OLDPWD)
		OLDPWD->str = ft_strjoin("OLDPWD=", shell->oldpwd);
}

/* 
 * Update la VE $_
 * Cette VE prends comme valeur le dernier argument passe en input
 * Mis a jour a chaque tour de boucle prompt
 * Exception pour env
*/
int	update_last_ve(t_args *list, t_args **env_list)
{
	char	*last_arg;

	if (ft_strcmp("env", list->str) == 0 && ft_strlen(list->str) == 3)
	{
		if (change_env_exp(env_list, "_", "/usr/bin/env") == 2)
			return (1);
		return (0);
	}
	while (list)
	{
		if (list->next == NULL)
		{
			last_arg = ft_strdup(list->str);
			if (!last_arg)
				return (1);
			break ;
		}
		list = list->next;
	}
	if (change_env_exp(env_list, "_", last_arg) == 2)
		return (1);
	return (0);
}
