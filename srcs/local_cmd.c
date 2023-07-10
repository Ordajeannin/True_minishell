/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalic <asalic@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 13:13:22 by asalic            #+#    #+#             */
/*   Updated: 2023/07/10 18:32:56 by asalic           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Cas speciaux de cd.
 * Check si pas d'arguments apres cd --> prends la valeur de $HOME.
 * Sinon, check si il y a ~ et renvoie donc a la racine $USER.
 * Sinon, tout va bien buf prends simplement la valeur du repertoire donne.
*/
static char	*cd_specialcase(t_args *list, t_shell *shell)
{
	char	*buf;

	if (!list->next)
		buf = getenv("HOME");
	else
	{
		if (ft_strncmp(list->next->str, "~", ft_strlen(list->next->str)) == 0)
			list->next->str = ft_strjoin("/mnt/nfs/homes/", shell->user);
		buf = list->next->str;
	}
	return (buf);
}

/* 
 * Change de repertoire.
 * Fonction a l'image de cd.
 * Change aussi PWD dans l'env.
*/
void	ft_cd(t_args *list, t_shell *shell, t_args *env_list)
{
	if (chdir(cd_specialcase(list, shell)) == -1)
	{
		ft_printf("bash: %s: %s: %s\n", list->str, list->next->str, \
			strerror(errno));
		return ;
	}
	else
	{
		change_env(&env_list, ft_strjoin("OLDPWD=", shell->pwd),
			ft_strjoin("OLDPWD=", shell->oldpwd));
		change_env(&env_list, ft_strjoin("PWD=", getcwd(NULL, 0)),
			ft_strjoin("PWD=", shell->pwd));
		shell->oldpwd = shell->pwd;
		shell->pwd = getcwd(NULL, 0);
	}
}

/* 
 * Affiche le repertoire courrant 
 * Fonction a l'image de 'pwd'
*/
void	ft_pwd(void)
{
	if (getcwd(NULL, 0) == NULL)
	{
		return ;
	}
	else
		ft_printf("%s\n", getcwd(NULL, 0));
}

/* 
 * Boucle principale d'unset
 * Cherche une VE et la supprime s'il la trouve
*/
static int	searchin_env(t_args **env_list, t_args *list)
{
	t_args	*current;
	t_args	*temp;

	current = *env_list;
	while (current)
	{
		if (ft_strncmp(list->next->str, current->next->str,
				ft_strlen(list->next->str)) == 0)
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
 * Fonction usent.
 * Supprime une variable d'environnement appele.
 * Change aussi les VE saved dans struct t_shell a NULL.
 * Si elle est vide ou n'existe pas, renvoie juste l'invite de commande.
*/

void	ft_unset(t_args *list, t_shell *shell, t_args *env_list)
{
	if (!list)
		return ;
	if (!searchin_env(&env_list, list))
		return ;
	else
		shell_change(shell, list->next->str, NULL);
	return ;
}

/* Fonction export.
 * Cherche d'abord si la VE existe deja.
 * Si oui, la modifie, dans env_list et dans shell.
 * Si non, la creee dans env_list seulement.
 * (Erreurs: appel a export+env direct == OK
 * 			 appel a export puis appel a env == KO
 * 			 change_env probleme)
*/
void	ft_export(t_args *list, t_shell *shell, t_args **env_list)
{
	char	*value;
	char	*v_env;
	char	*string;

	string = malloc((ft_strlen(list->next->str) +1) * sizeof(char));
	string = list->next->str;
	v_env = ft_strdupto_n(list->next->str, '=');
	ft_printf("V_ENV = %s\n", v_env);
	value = ft_strdup_from(list->next->str, '=');
	ft_printf("V_ENV = %s\nVALUE = %s\n", v_env, value);
	if (!change_env(env_list, value, v_env))
	{
		add_arg(env_list, string, 0);
	}
	else
		shell_change(shell, v_env, value);
}
